int result;

int sum_of(int x, int y) { // function declaration
   int z = 0;
   z = x + y;
   return z; // return the value
}

void setup () {
   result = 0; // group of statements
}

void loop () {
   result = sum_of(5, 6); // function call
}
