#include "paging.h"
#include "kheap.h"

u32 *frames;
u32 nframes;
page_dir_t *current_dir;
page_dir_t* kernel_dir;
extern u32 placement_addr;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

// set a bit in the frames bitset
static void set_frame(u32 frame_addr){
    u32 frame = frame_addr/0x1000;
    u32 idx = INDEX_FROM_BIT(frame);
    u32 off = OFFSET_FROM_BIT(frame);

    frames[idx] |= (0x1 << off);
}



static void clear_frame(u32 frame_addr){
    u32 frame = frame_addr/0x1000;
    u32 idx = INDEX_FROM_BIT(frame);
    u32 off = OFFSET_FROM_BIT(frame);

    frames[idx] &= ~(0x1 << off);
}

static u32 test_frame(u32 frame_addr){
    u32 frame = frame_addr/0x1000;
    u32 idx = INDEX_FROM_BIT(frame);
    u32 off = OFFSET_FROM_BIT(frame);

    return (frames[idx] & (0x1 << off));
}

static u32 first_frame(){
    u32 i, j;
    for(i=0; i<INDEX_FROM_BIT(nframes); i++){

        if(frames[i] != 0xFFFFFFFF){

            for(j=0; j<32; j++){
                
                u32 to_test = 0x1 << j;

                if (!(frames[i]&to_test)){
                    return i*4*8+j;
                }
            }
        }
    }
}

void alloc_frame(page_t *page, int is_kernel, int is_writ){
    if (page->frame != 0){
        return;
    } else {
        u32 idx = first_frame();
        if (idx == (u32)-1){ // if we don't have any frames left
            PANIC("No free frames!");
        }
        set_frame(idx*0x1000);
        page->present = 1;
        page->rw = (is_writ)?1:0;
        page->user = (is_kernel)?0:1;
        page->frame = idx;
    }
}

void free_frame(page_t *page){
    u32 frame;
    if (!(frame==page->frame)){
        return;
    } else {
        clear_frame(frame);
        page->frame = 0x0;
    }
}

void init_paging(){
    u32 mem_end = 0x1000000;
    
    nframes = mem_end / 0x1000;
    frames = (u32*)kmalloc(INDEX_FROM_BIT(nframes));
    // memset(frames, 0, INDEX_FROM_BIT(nframes));

    kernel_dir = (page_dir_t*)kmalloc_a(sizeof(page_dir_t));
    kernel_dir->physical_addr = (u32)kernel_dir->tables_physical;
    // memset(kernel_dir, 0, sizeof(page_dir_t));
    current_dir = kernel_dir;

    int i = 0;
    while (i<placement_addr){
        alloc_frame(get_page(i, 1, kernel_dir), 0, 0);
        i+=0x1000;
    }

    register_interrupt_handler(14, page_fault);
    switch_page_dir(kernel_dir);
}


// void switch_page_dir(page_dir_t *dir){
//     page_dir_t *current_dir = dir;
//     asm volatile("mov %0, %%cr3"::"r"(&dir->tables_physical));
//     u32 cr0;
//     asm volatile("mov %%cr0, %0": "=r"(cr0));
//     cr0 |= 0x80000000;
//     asm volatile("mov %0, %%cr0":: "r"(cr0));
// }

void switch_page_dir(page_dir_t *dir)
{
    current_dir = dir;
    asm volatile("mov %0, %%cr3":: "r"(dir->physical_addr));
    u32 cr0;
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x00000001; // Enable paging!
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

// page_t *get_page(u32 addr, int make, page_dir_t *dir){
//     addr /= 0x1000;

//     u32 table_idx = addr / 1024;
//     if(dir->tables[table_idx]){
//         return &dir->tables[table_idx]->pages[addr%1024];
//     } else if(make) {
//         u32 tmp;
//         dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
//         memset(dir->tables[table_idx], 0, 0x1000);
//         dir->tables_physical[table_idx] = tmp | 0x7;
//         return &dir->tables[table_idx]->pages[addr%1024];
//     } else {
//         return 0;
//     }
// }

page_t *get_page(u32 address, int make, page_dir_t *dir)
{
   // Turn the address into an index.
   address /= 0x1000;
   // Find the page table containing this address.
   u32 table_idx = address / 1024;
   if (dir->tables[table_idx]) // If this table is already assigned
   {
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else if(make)
   {
       u32 tmp;
       dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
       memset(dir->tables[table_idx], 0, 0x1000);
       dir->tables_physical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
       return &dir->tables[table_idx]->pages[address%1024];
   }
   else
   {
       return 0;
   }
} 

void page_fault(registers_t regs){
    // u32 faulting_addr;
    // asm volatile("mov %%cr2, %0" : "=r" (faulting_addr));
    // int present = !(regs.err_code & 0x1);
    // int rw =        regs.err_code & 0x2;
    // int us =        regs.err_code & 0x4;
    // int reserved =  regs.err_code & 0x8;
    // int id =        regs.err_code & 0x10;

    print("Page fault! (");
    // if (present) print("present ");
    // if (rw) print("read-only ");
    // if (us) print("user-mode ");
    // if (reserved) print("reserved ");
    // print(") at 0x");
    // // print_hex(faulting_addr);
    // print("\n");
    // PANIC("Page fault!");
}