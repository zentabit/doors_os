void memcpy(char* src, char* dst, int nbytes){
    for (int i = 0; i < nbytes; i++){
        *(dst + i) = *(src + i);
    }
}

// really unsure if this works
// void* memset(char* start, char value, int nbytes){
//     char *address = start;
//     for(int i = 0; i < nbytes; i++){
//         *(address + i) = value;
//     }
// }

void memory_set(unsigned char *dest, unsigned char val, unsigned int len) {
    unsigned char *temp = (unsigned char *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

void *memset(void *s, int c, int n)
{
    unsigned char* p=s;
    while(n--)
        *p++ = (unsigned char)c;
    return s;
}

void PANIC(char msg[]){
    print(msg);
    while(1){}
}

// char dec_to_ascii(){

// }