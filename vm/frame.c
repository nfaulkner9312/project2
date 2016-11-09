/* The frame table implementation */

#include "vm/frame.h"
#include "vm/swap.h"
#include "vm/page.h"
#include "filesys/file.h"
#include "userprog/pagedir.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "threads/vaddr.h"
#include "threads/palloc.h"
#include <string.h>

void init_frame_table(void) {
    list_init(&frame_table);
}

void* frame_allocate( struct s_page_table_entry *spte) {

    void *frame = palloc_get_page(PAL_USER);
        /* add frame to frame table */
   if(!frame) {
        /* evict frame to make room */
        frame=evict_frame();
        /* if frame could not be evicted than swap is full, PANICCC!!! */
    if(!frame){
            PANIC("EVERYBODY PANIC!!!! THE SWAP DISK IS FULL!!!!");
        }
    }

   struct frame_table_entry *fte = malloc(sizeof(struct frame_table_entry));
   fte->frame = frame;
   fte->spte = spte;
   fte->spte->my_fte=fte;/*linked spte and fte*/
    
   fte->spte->is_resident=true;/*spte resides in frame*/

   list_push_back(&frame_table, &fte->elem);
    
   return frame;
}

void free_frame(struct frame_table_entry* fte) {
/*free specific frame, delete fte, update spte, potentially write back to disk?*/



}
void* evict_frame(){
    /* free up a frame(move to swap), delete fte, update spte, return pointer to empty frame */

    /* this is probably fairly involved, check dirty bits and such */
    void* empty_frame;

    /* grab oldest frame from frame_table, potentially using dirty & accessed */
    struct frame_table_entry* frame = get_frame_to_evict();
    
    /* move frame from memory into swap slot, which updates spte within frame */
    move_to_swap(frame);
    empty_frame = frame->frame;/*new pointer points to beginning of frame*/
    free(frame);
    

    /* pagedir_is_dirty */
    /* pagedir_is_accessed */
    
    /* clear contents of frame */
    memset(empty_frame, 0, PGSIZE);

    return empty_frame;
}

struct frame_table_entry* get_frame_to_evict(){

    struct list_elem* e;
    struct frame_table_entry* fte;
    
    e=list_begin(&frame_table);
    fte = list_entry(e,struct frame_table_entry, elem);

    return fte;

 /*   for(e=list_begin(&frame_table_list); e!= list_end(&frame_table_list); e=list_next(e)){


}

return */
}
