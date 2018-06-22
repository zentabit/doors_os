void memcpy(char* src, char* dst, int nbytes){
    for (int i = 0; i < nbytes; i++){
        *(dst + i) = *(src + i);
    }
}

// void memset(char* start, int nbytes, char value){

// }