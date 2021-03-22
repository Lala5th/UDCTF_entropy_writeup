# Decompilation

First let's depcompile our program with ghidra. After deobfuscating we get main:

```c
long main(undefined8 param_1,long param_2)

{
  code cVar1;
  byte bVar2;
  long in_FS_OFFSET;
  int offset;
  ulong target;
  code *local_28;
  long stack_cookie;

  stack_cookie = *(long *)(in_FS_OFFSET + 0x28);
  puts(*(char **)(param_2 + 8));
  target = **(ulong **)(param_2 + 8) & 0xffffffffffff;
  printf("%lu\n",target);
  offset = 0;
  while (offset < 100) {
    cVar1 = FUN_00301020[offset];
    bVar2 = shuffle(&target);
    *(byte *)(FUN_00301020 + offset) = bVar2 ^ (byte)cVar1;
    offset = offset + 1;
  }
  if (target == 0xfd94e6e84a0a) {
    puts("looking good");
  }
  local_28 = FUN_00301020;
  FUN_00301020();
  if (stack_cookie != *(long *)(in_FS_OFFSET + 0x28)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return 0;
}
```

We see that we want our target wariable to be 0xfd94e6e84a0a by the end. We also see that for 100 times shuffle is called on this variable. The return of this function is used to update a function in the program, which is called at the end of main. This will probably print out the flag if the correct argument is supplied. Looking at shuffle reveals

```c
ulong shuffle(ulong *param_1)

{
  *param_1 = *param_1 * 0x5deece66d + 0xb & 0xffffffffffff;
  return *param_1;
}
```

This is just a multiplication with 0x5deece66d and adding the result to 0xb. As the result is bitwise and-ed with 0xffffffffffff.

# Modulo arithmetic

The above problem is equvivalent to reversing multiplication and addition on modulo 0x1000000000000. Reversing these on modulo arithmetic requires the inverse of both these. Reversing addition of 0xb is equvivalent to adding 0x1000000000000 - 0xb + 1. For reversing multiplication we need to find the multiplicative inverse of 0x5deece66d on 0x1000000000000. In this case we can use online tools to find that this is 246154705703781.

# Reversing

We can write a simple program on these principles to reverse this process:

```cpp
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
```

Running this gives us the result 0x483d34347a46. We can see that this needs to be passed as a string argument, which is then interpreted as a ulong. Writing out the bytes in little endian format we get: 46 7a 34 34 3d 48 00 00 -> Fz44=H .

Passing this as an argument produces our flag: UDCTF{wh4t_c4n_I_say_luv_crypt0}
