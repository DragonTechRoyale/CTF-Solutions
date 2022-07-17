void protect(char *pointer_to_name_inputted)
{
  size_t second_length_of-name_inputted;
  size_t length_of_string;
  int in_GS_OFFSET;
  uint i_index;
  uint j_index;
  undefined4 local_127;
  undefined4 local_123;
  undefined4 local_11f;
  undefined4 local_11b;
  undefined4 local_117;
  undefined2 local_113;
  undefined local_111;
  char local_buffer [256];
  int local_10;
  
                    /* Protect probably does this:
                       replace all bytes that are #&;`'\"|*?~<>^()[]{}$\\, with ♥ */
  local_10 = *(int *)(in_GS_OFFSET + 0x14);
  local_127 = 0x603b2623;
  local_123 = 0x2a7c2227;
  local_11f = 0x3e3c7e3f;
  local_11b = 0x5b29285e;
  local_117 = 0x247d7b5d;
  local_113 = 0x2c5c;
  local_111 = 0;
  i_index = 0;
  while( true ) {
    length_of_string = strlen(pointer_to_name_inputted);
    if (length_of_string <= i_index) break;
    j_index = 0;
    while( true ) {
      length_of_string = strlen((char *)&local_127);
      if (length_of_string <= j_index) break;
      if (pointer_to_name_inputted[i_index] == *(char *)((int)&local_127 + j_index)) {
        strcpy(local_buffer,pointer_to_name_inputted + i_index + 1);
                    /* 0x00a599e2 = "♥" */
        *(undefined4 *)(pointer_to_name_inputted + i_index) = 0xa599e2;
        length_of_string = strlen(local_buffer);
        second_length_of-name_inputted = strlen(pointer_to_name_inputted);
        memcpy(pointer_to_name_inputted + second_length_of-name_inputted,local_buffer,
               length_of_string);
      }
      j_index = j_index + 1;
    }
    i_index = i_index + 1;
  }
  if (local_10 != *(int *)(in_GS_OFFSET + 0x14)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}

void protect(char *pointer_to_name_inputted)
{
  size_t second_length_of-name_inputted;
  size_t length_of_string;
  int in_GS_OFFSET;
  uint i_index;
  uint j_index;
  undefined4 local_127;
  undefined4 local_123;
  undefined4 local_11f;
  undefined4 local_11b;
  undefined4 local_117;
  undefined2 local_113;
  undefined local_111;
  char local_buffer [256];
  int local_10;
  
                    /* Protect probably does this:
                       replace all bytes that are #&;`'\"|*?~<>^()[]{}$\\, with ♥ */
  local_10 = *(int *)(in_GS_OFFSET + 0x14);
  local_127 = 0x603b2623;
  local_123 = 0x2a7c2227;
  local_11f = 0x3e3c7e3f;
  local_11b = 0x5b29285e;
  local_117 = 0x247d7b5d;
  local_113 = 0x2c5c;
  local_111 = 0;
  i_index = 0;
  while( true ) {
    length_of_string = strlen(pointer_to_name_inputted);
    if (length_of_string <= i_index) break;
    j_index = 0;
    while( true ) {
      length_of_string = strlen((char *)&local_127);
      if (length_of_string <= j_index) break;
      if (pointer_to_name_inputted[i_index] == *(char *)((int)&local_127 + j_index)) {
        strcpy(local_buffer,pointer_to_name_inputted + i_index + 1);
                    /* 0x00a599e2 = "♥" */
        *(undefined4 *)(pointer_to_name_inputted + i_index) = 0xa599e2;
        length_of_string = strlen(local_buffer);
        second_length_of-name_inputted = strlen(pointer_to_name_inputted);
        memcpy(pointer_to_name_inputted + second_length_of-name_inputted,local_buffer,
               length_of_string);
      }
      j_index = j_index + 1;
    }
    i_index = i_index + 1;
  }
  if (local_10 != *(int *)(in_GS_OFFSET + 0x14)) {
                    /* WARNING: Subroutine does not return */
    __stack_chk_fail();
  }
  return;
}
