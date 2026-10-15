#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct header {
  uint64_t size;
  struct header *next;
  int id;
};

void initialize_block(struct header *block, uint64_t size, struct header *next,
                      int id) {
  block->size = size;
  block->next = next;
  block->id = id;
}

// First fit: return the ID of the first block (in list order) that is big
// enough, or -1 if none is.
int find_first_fit(struct header *free_list_ptr, uint64_t size) {
  for (struct header *cur = free_list_ptr; cur != NULL; cur = cur->next) {
    if (cur->size >= size) {
      return cur->id;
    }
  }
  return -1;
}

// Best fit: among blocks that are big enough, return the ID of the smallest.
int find_best_fit(struct header *free_list_ptr, uint64_t size) {
  int best_fit_id = -1;
  uint64_t best_size = 0;
  for (struct header *cur = free_list_ptr; cur != NULL; cur = cur->next) {
    if (cur->size >= size && (best_fit_id == -1 || cur->size < best_size)) {
      best_fit_id = cur->id;
      best_size = cur->size;
    }
  }
  return best_fit_id;
}

// Worst fit: among blocks that are big enough, return the ID of the largest.
int find_worst_fit(struct header *free_list_ptr, uint64_t size) {
  int worst_fit_id = -1;
  uint64_t worst_size = 0;
  for (struct header *cur = free_list_ptr; cur != NULL; cur = cur->next) {
    if (cur->size >= size && (worst_fit_id == -1 || cur->size > worst_size)) {
      worst_fit_id = cur->id;
      worst_size = cur->size;
    }
  }
  return worst_fit_id;
}

int main(void) {

  struct header *free_block1 = (struct header*) malloc(sizeof(struct header));
  struct header *free_block2 = (struct header*) malloc(sizeof(struct header));
  struct header *free_block3 = (struct header*) malloc(sizeof(struct header));
  struct header *free_block4 = (struct header*) malloc(sizeof(struct header));
  struct header *free_block5 = (struct header*) malloc(sizeof(struct header));

  initialize_block(free_block1, 6, free_block2, 1);
  initialize_block(free_block2, 12, free_block3, 2);
  initialize_block(free_block3, 24, free_block4, 3);
  initialize_block(free_block4, 8, free_block5, 4);
  initialize_block(free_block5, 4, NULL, 5);

  struct header *free_list_ptr = free_block1;

  int first_fit_id = find_first_fit(free_list_ptr, 7);
  int best_fit_id = find_best_fit(free_list_ptr, 7);
  int worst_fit_id = find_worst_fit(free_list_ptr, 7);

  printf("The ID for First-Fit algorithm is: %d\n", first_fit_id);
  printf("The ID for Best-Fit algorithm is: %d\n", best_fit_id);
  printf("The ID for Worst-Fit algorithm is: %d\n", worst_fit_id);

  free(free_block1);
  free(free_block2);
  free(free_block3);
  free(free_block4);
  free(free_block5);

  return 0;
}

/*
 * Part 2: Coalescing contiguous free blocks (pseudo-code)
 *
 * coalesce(free_list, freed):
 *     freed_start = address of freed
 *     freed_end = freed_start + total_size(freed)
 *
 *     prev = NULL; // free block that ends exactly at freed_start
 *     next = NULL; // free block that starts exactly at freed_end
 *
 *     for each block b in free_list:
 *         if end_address(b) == freed_start:
 *             prev = b;
 *         if start_address(b) == freed_end:
 *             next = b;
 *         if prev != NULL and next != NULL:
 *             break; // found both neighbours
 *
 *     if prev == NULL and next == NULL:
 *         // Case 1: no adjacent free block
 *         insert freed into free_list (e.g. at the head)
 *
 *     else if prev != NULL and next == NULL:
 *         // Case 2: free block immediately BEFORE freed
 *         prev.size = prev.size + total_size(freed)
 *
 *     else if prev == NULL and next != NULL:
 *         // Case 3: free block immediately AFTER freed
 *         freed.size = freed.size + total_size(next)
 *         remove next from free_list
 *         insert freed into free_list (in next's old position)
 *
 *     else:
 *         //Case 4: free blocks both BEFORE and AFTER freed
 *         prev.size = prev.size + total_size(freed) + total_size(next)
 *         remove next from free_list// prev stays where it is
 *
 * Example (layout: [x free][y used][z freed][w free]):
 * x ends where z starts  -> prev = x
 * z ends where w starts  -> next = w
 * Case 4: x absorbs z and w; w is removed from the list.
 * Result: one large free block starting at x.
 */