boolean string_is_int(String str) {
  int n = str.length();

  if (n == 0){
    return false;
  }
  
  for (int i = 0; i < n; i++) {
    if (!(isDigit(str[i]) || str[i] == '-')) {
      return false;
    }
  }

  return true;
}

boolean string_is_float(String str) {
  int n = str.length();

  if (n == 0){
    return false;
  }
  
  for (int i = 0; i < n; i++) {
    if (!(isDigit(str[i]) || str[i] == '-' || str[i] == '.')) {
      return false;
    }
  }

  return true;
}

int count_char(String str, char c) {
  //Returns how many of char c is in string str
  int count = 0;
  int n = str.length();

  for (int i = 0; i < n; i++) {
    if (str[i] == c) {
      count++;
    }
  }

  return count;
}
