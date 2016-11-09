/*#include "vm/frame.h"
#include "vm/page.h"
#include "threads/palloc.h"*/
#include <list.h>
/*#include "devices/block.h"*/

/*1. slots need to be freed upon process termination*/

    struct block* my_block; 
struct swap_slot{
    struct list_elem elem;
    uint32_t bs_index;
    bool free;
};

/*swap table will be accessed through page fault handler almost exclusively, potentially it may be interacted through process.c but most likely we can use just localize all of it's access to page.c*/

struct list swap_table;

void init_swap_table(void);

/*we need to split the following processes between page and swap*/

void move_to_swap(struct frame_table_entry*);/*copy frame contents to swap disk, switch booleans within SPTE, update swap_slot*/

void remove_from_swap(struct frame_table_entry*, struct s_page_table_entry*);/*copy frame contents(into frame that is already allocated to SPTE), clear and free swap_slot, maybe update booleans in SPTE*/
void free_slot(struct swap_slot*);/*simply evict swap slot, no data copying*/
struct swap_slot* get_empty_slot(void);
