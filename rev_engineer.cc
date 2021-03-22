#include <iostream>

const ulong addition_inv = 0xffffffffffff - 0xb + 1;
const ulong mult_inv = 246154705703781;

void rev_mod(ulong& var){
	var = var + addition_inv & 0xffffffffffff;
	var = var * mult_inv & 0xffffffffffff;
}


int main(){
	ulong target = 0xfd94e6e84a0a;
	size_t i = 0;
	while(i<100){
		rev_mod(target);
		i++;
	}
	ulong res = target;
	printf("0x%lx\n",target);
	return 0;
}
