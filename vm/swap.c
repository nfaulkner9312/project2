
#include "vm/frame.h"
#include "vm/page.h"
#include "vm/swap.h"
#include "filesys/file.h"
#include "userprog/pagedir.h"
#include "threads/malloc.h"
#include "threads/palloc.h"
#include "devices/block.h"
#include <stdio.h>


void init_swap_table(void){
    list_init(&swap_table);
    my_block=block_get_role(BLOCK_SWAP);
    /* populate swap table with every swap slot on the disk */
    struct swap_slot* ss=malloc(sizeof(struct swap_slot));
    ss->bs_index=0;
    list_push_back(&swap_table, &ss->elem);
    if(my_block==NULL){
        PANIC("SWAP BLOCK DEVICE NOT INITIALIZED PROPERLY!!!");
    }
    
    printf("SWAP BLOCK DEVICE INITIALIZED\n");
};


void move_to_swap(struct frame_table_entry* fte){
/*copy frame into swap slot*/
/*update spte from within fte*/
/*connect spte to swap_slot, return fte*/
struct swap_slot* new_slot=get_empty_slot();
void* copy_from = fte->frame;
/*write in 8 separate chunks/sectors*/

new_slot->free=false;
int sectors=0;
while(sectors<8){
block_write(my_block,new_slot->bs_index*8+sectors,copy_from);
sectors++;
}/*sector written*/

fte->spte->is_swap=true;
fte->spte->is_resident=false;
fte->spte->ss=new_slot;
fte->spte=NULL;

}

struct swap_slot* get_empty_slot(){

    struct swap_slot* empty_slot;
    struct list_elem* e;
    bool has_swap=false;
    for(e=list_begin(&swap_table); e!= list_end(&swap_table); e=list_next(e))
    {
        empty_slot=list_entry(e,struct swap_slot, elem);
        if(empty_slot->free){
            has_swap=true;
            break;
        }
    }
    if(!has_swap){/*allocate new swap slot*/
    empty_slot=malloc(sizeof(struct swap_slot));
        if(empty_slot!=NULL)
        {
            struct swap_slot* last_slot=list_entry(list_back(&swap_table),struct swap_slot, elem);
            has_swap=true;
            empty_slot->bs_index=last_slot->bs_index+1;
        }
    }
    if(!has_swap){
        PANIC("PANIC! NO MORE SWAP SLOTS");
    }
    return empty_slot;

}

void free_slot(struct swap_slot* ss){

}

void remove_from_swap(struct frame_table_entry* fte, struct s_page_table_entry* spte){
void* frame=fte->frame;
int sector=0;

while(sector<8){
block_read(my_block,spte->ss->bs_index*8+sector,frame);
sector++;
}

spte->ss->free=true;
spte->ss=NULL;
spte->is_resident=true;
spte->is_swap=false;



/*copy from swap into allocated frame */
/* update spte through fte */


}
