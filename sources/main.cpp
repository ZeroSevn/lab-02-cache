#include <cstddef>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>


// [[void const*], [void const*], [void const*], ...]*
void const* * directOrder(size_t const length) { // 1* mean pointers 2* - mean array
  void const* * array = new void const*[length];
  for (size_t i = 0; i < length - 16; i=i+16) {
    array[i] = &array[i + 16];
  }
  array[length - 16] = nullptr;

  return array;
}

void const* *rdirectOrder(size_t const length){
  void const* * array = new void const*[length];
  array[0]=&array[length-16];
  for (size_t i = 32; i < length; i=i+16) {
    array[i] = &array[i - 16];
  }
  array[16] = nullptr;

  return array;
}

void const* *randomOrder(size_t const length) {
  void const** array = new void const*[length];
  srand(length - 1);
  std::set<size_t> used_random{0u};

  size_t random;
  void const* * current = array;
  for (size_t i = 0; i < length - 16; i=i+16) {

    random = rand() % length;

    while (used_random.find(random) != used_random.end())
      random = (random + 1) % length;
    used_random.insert(random);
    *current = &array[random]; // ссылка на то что хронить (значене)
    current = static_cast<void const**>(array + random);// переходим к элементу в котором лежит (значение)
  }
  current = nullptr; // the last in array


  return array;
}



int main() {
//  using Duration = std::chrono::duration<unsigned long>;

  // L1 = 32 k byte =
  // l2 = 256 k byte = 256 * 1024 / 4
  // l3 = 8 m byte = 12 (1024 * 1024)/4 byte

  /*int * arr_0 = new int[32*1024*1024];
  int * arr_1 = new int[64*1024];

  int * arr_5 = new int[12*1024*1024];
   */
  //1 - 32k шаг в двойку 2^x = 64
  //int *p;
  //std::cout << sizeof(int); // = 4;
  // 8388608
  std::vector<size_t> cache{16u,32u,64u,128u,256u,512u,1024u,2048u,4096,8192u,12288u};
  for (size_t i = 0; i < cache.size();i++){
    auto const tested = directOrder((cache[i] << 10u) / sizeof(void *));
    auto const tested_2 = rdirectOrder((cache[i] << 10u) / sizeof(void *));
    auto const tested_3 = randomOrder((cache[i] << 10u) / sizeof(void *));

    void const* const* current;  // second * is mean array;
    void const* const* current_2;  // second * is mean array;
    void const* const* current_3;  // second * is mean array;


    for (size_t j = 0u; j < 100u; ++j) {
      current = tested;                                                   // warmup
      while (current) current = static_cast<void const* const*>(*current) ;// warmup
    }

    // start
    auto const start_time = std::chrono::steady_clock::now();
    for (size_t j = 0u; j < 1000u; ++j) {
      current = tested;
      while (current) {
        current = static_cast<void const* const*>(*current);
      }
    }
    auto const end_time = std::chrono::steady_clock::now();
    auto const duration = std::chrono::duration_cast<std::chrono::nanoseconds>((end_time - start_time) / 1000u).count();

    for (size_t j = 0u; j < 100u; ++j) {
      current_2 = tested_2;                                                   // warmup
      while (current_2) current_2 = static_cast<void const* const*>(*current_2);// warmup
    }

    // start
    auto const start_time_2 = std::chrono::steady_clock::now();
    for (size_t j = 0u; j < 1000u; ++j) {
      current_2 = tested_2;
      while (current_2) {
        current_2 = static_cast<void const* const*>(*current_2);
      }
    }
    auto const end_time_2 = std::chrono::steady_clock::now();
    auto const duration_2 = std::chrono::duration_cast<std::chrono::nanoseconds>((end_time_2 - start_time_2) / 1000u).count();

    //
    for (size_t j = 0u; j < 100u; ++j) {
      current_3 = tested_3;                                                   // warmup
      while (current_3) current_3 = static_cast<void const* const*>(*current_3);// warmup
    }

    // start
    auto const start_time_3 = std::chrono::steady_clock::now();
    for (size_t j = 0u; j < 1000u; ++j) {
      current_3 = tested_3;
      while (current_3) {
        current_3 = static_cast<void const* const*>(*current_3);
      }
    }
    auto const end_time_3 = std::chrono::steady_clock::now();
    auto const duration_3 = std::chrono::duration_cast<std::chrono::nanoseconds>((end_time_3 - start_time_3) / 1000u).count();





    // end
    std::cout << "For " << cache[i] << " direct  array= " << duration << std::endl;
    std::cout << "For " << cache[i] << " reverse array= "<< duration_2 << std::endl;
    std::cout << "For " << cache[i] << " random  array= " << duration_3 << std::endl;
    std::cout << "_______________________________" << std::endl;
  }



  return 0;
}
