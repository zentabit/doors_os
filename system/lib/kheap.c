#include "kheap.h"

u32 placement_addr = 0x10000;

// u32 kmalloc(u32 sz, int align, u32 *phys){
//     if (align && (placement_addr & 0xfffff000)){
//         placement_addr &= 0xfffff000;
//         placement_addr += 0x1000;
//     }
//     if (phys){
//         *phys = placement_addr;
//     }
//     u32 tmp = placement_addr;
//     placement_addr += sz;
//     return tmp;
// }

u32 kmalloc(u32 sz, int align, u32 *phys)
{
  if (align == 1 && (placement_addr & 0xFFFFF000)) // If the address is not already page-aligned
  {
    // Align it.
    placement_addr &= 0xFFFFF000;
    placement_addr += 0x1000;
  }
  if (phys)
  {
    *phys = placement_addr;
  }
  u32 tmp = placement_addr;
  placement_addr += sz;
  return tmp;
} 

u32 kmalloc_a(u32 sz){
    kmalloc(sz, 1, 0);
}

u32 kmalloc_ap(u32 sz, u32 *phys){
    kmalloc(sz, 1, phys);
}

u32 kmalloc_p(u32 sz, u32 *phys){
    kmalloc(sz, 0, phys);
}