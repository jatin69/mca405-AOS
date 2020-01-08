/*

  todo :
  review code
  clean code
  comment code
  test code

  https://en.cppreference.com/w/cpp/language/rule_of_three

  Insane article - https://codereview.stackexchange.com/a/149939

  Phase 1: carry out things that might throw, but don't affect any existing data
  if they do. Phase 2: carry out operation that affect existing data, but can't
  throw.
*/

// TODO :
// constructor for all 3 - default and param.
// overload assisnment and equality operator for all 3
// overload big FIvE
// write comments as below

// remove this
// This file declares a `buffer`.
// A `buffer` is made up of two parts - buffer header & data array
// buffer header contains a lot of fields including the status field.
// data array corresponds to data in a logical disk block.

/**
 *  @brief Erases an element from an %unordered_set.
 *  @param  __position  An iterator pointing to the element to be erased.
 *  @return An iterator pointing to the element immediately following
 *          @a __position prior to the element being erased. If no such
 *          element exists, end() is returned.
 *
 *  This function erases an element, pointed to by the given iterator,
 *  from an %unordered_set.  Note that this function only erases the
 *  element, and that if the element is itself a pointer, the pointed-to
 *  memory is not touched in any way.  Managing the pointer is the user's
 *  responsibility.
 */

#include <bits/stdc++.h>
using namespace std;

/** Toggle Development Mode
 *
 * Possible Values : 0 / 1
 **/

#define DEVELOPMENT_MODE 1

/** Disk Block Size
 *
 * In dev mode, size 8 is enough
 * In production mode, size = 1KB = 1024 Bytes = 1024 chars
 **/

#if DEVELOPMENT_MODE == 1
#define DISK_BLOCK_SIZE 10
#else
#define DISK_BLOCK_SIZE 1024
#endif

/** structure : BufferHeader
 *
 * BufferHeader is made of two fields which together uniquely identifies the
 * buffer in the hash queue.
 *
 * BufferHeader should be hashable, in order to be used in a hash map.
 **/

struct BufferHeader {

  int device_number;
  int block_number;

  BufferHeader() : device_number{-1}, block_number{-1} {}

  BufferHeader(int _device_number, int _block_number)
      : device_number{_device_number}, block_number{_block_number} {}

  // overload == operator : required for hashing
  inline bool operator==(const BufferHeader &rhs) const {
    return (device_number == rhs.device_number) &&
           (block_number == rhs.block_number);
  }

  inline bool operator!=(const BufferHeader &rhs) const {
    return !(*this == rhs);
  }

  /** Overloaded operator() for `BufferHeader`
   *
   * @brief A hash function to make the struct `BufferHeader` hashable
   * @param  __position  A `BufferHeader` object
   * @return A suitable hash value
   *
   * Requirement -
   * We need a suitable hash function that uniquely maps two integers
   * of `BufferHeader` into a single integer i.e.  ( N x N -> N )
   *
   * Approaches -
   * 1. we can simply xor -
   * but there's no guarantee it will work
   * int hash = (std::hash<int>()(k1)) ^ (std::hash<int>()(k2)) ;
   *
   * 2. cantors pairing function -
   * Source -
   * https://en.wikipedia.org/wiki/Pairing_function#Cantor_pairing_function But
   * this soon goes out of bound for large elements, and it does not utilise
   * sample space properly. Also there's a lot of calculation going on. int hash
   * = k2 + (0.5 * (k1+k2) * (k1+k2+1));
   *
   * 3. Szudzik's function - http://szudzik.com/ElegantPairing.pdf
   * Performs better than above options, but hard to remember
   * Extensible, but becomes even more complex when
   * the number of elements in the tuple (k1, k2, k3, .. ) increases.
   * int hash = (k1 >= k2) ? ((k1 * k1) + k1 + k2) : (k1 + (k2 * k2));
   *
   * 4. Dan Bernstein's hash function, just with different coefficients
   * No one knows why these specific coefficients work so well, they just do.
   * Source SO answer - https://stackoverflow.com/a/1646913
   * suggested by Josh Bloch in Effective Java.
   * Extensible to any number of elements, and easy to remember.
   *
   * int hash = 17;
   * hash = hash * 31 + firstField.GetHashCode();
   * hash = hash * 31 + secondField.GetHashCode();
   * return hash;
   *
   */

  std::size_t operator()(const BufferHeader &buffer_header) const noexcept {
    // Dan Bernstein's hash function
    int hash = 17;
    hash = hash * 31 + std::hash<int>()(buffer_header.device_number);
    hash = hash * 31 + std::hash<int>()(buffer_header.block_number);
    // cout <<"\nxxxxxxxxxx" << hash << " xxxxxxxxxx\n";
    return hash;
  }

  friend void swap(BufferHeader &first, BufferHeader &second) noexcept {
    using std::swap;
    swap(first.device_number, second.device_number);
    swap(first.block_number, second.block_number);
  }
};

/** structure : Buffer
 *
 * Buffer consists of a lot of properties. Some of them are -
 *
 * 1. buffer header - group of device num and block num
 *
 * 2. status field - lowermost 5 bits constitue status. One or more bits can be
 *active at once. 0th bit - buffer is locked (1) / unlocked (0) 1st bit - buffer
 *contains valid data (1) 2nd bit - needs delayed write (1) 3rd bit - kernel
 *is currently reading or writing contents of buffer to disk (1) 4th bit - a
 *process is waiting for process to become free (1) 5th to 32nd bit - set to
 *0 as no active usage
 *
 * 3. data area size - tells size of data buffer's data area points to.
 *    This should atleast be equal to DISK_BLOCK_SIZE
 *
 * 4. data area - pointer to data area. Most common size is 1KB
 *
 * 5. pointer to prev buffer on hash queue
 * 6. pointer to next buffer on hash queue
 *
 * 7. pointer to prev buffer on free list
 * 8. pointer to next buffer on free list
 *
 **/

struct Buffer {
  BufferHeader buffer_header;
  unsigned int status;
  size_t data_area_size;
  char *data_area;
  Buffer *prev_buffer_on_hash_queue;
  Buffer *next_buffer_on_hash_queue;
  Buffer *prev_buffer_on_free_list;
  Buffer *next_buffer_on_free_list;

  Buffer()
      : buffer_header{BufferHeader()}, status{0},
        data_area_size{DISK_BLOCK_SIZE}, data_area{new char[DISK_BLOCK_SIZE]},
        prev_buffer_on_hash_queue{nullptr}, next_buffer_on_hash_queue{nullptr},
        prev_buffer_on_free_list{nullptr}, next_buffer_on_free_list{nullptr} {}

  Buffer(BufferHeader _buffer_header, unsigned int _status,
         size_t _data_area_size = DISK_BLOCK_SIZE, const char *_data_area = "",
         Buffer *_prev_buffer_on_hash_queue = nullptr,
         Buffer *_next_buffer_on_hash_queue = nullptr,
         Buffer *_prev_buffer_on_free_list = nullptr,
         Buffer *_next_buffer_on_free_list = nullptr)
      : buffer_header{_buffer_header}, status{_status},
        data_area_size{_data_area_size < DISK_BLOCK_SIZE ? DISK_BLOCK_SIZE
                                                         : _data_area_size},
        data_area{new char[data_area_size]},
        prev_buffer_on_hash_queue{_prev_buffer_on_hash_queue},
        next_buffer_on_hash_queue{_next_buffer_on_hash_queue},
        prev_buffer_on_free_list{_prev_buffer_on_free_list},
        next_buffer_on_free_list{_next_buffer_on_free_list} {
    std::memcpy(data_area, _data_area, data_area_size);
  }

  /** Swap function
   *
   * Simply swap all data members using appropriate swaps
   *
   * Why is it made friend ?
   * Reasoning - https://stackoverflow.com/a/5695855
   *
   * A friend function defined inside a class isn't a member function -
   * it exists in the enclosing namespace scope.
   * In general, such a function won't be seen in that scope by external code,
   * but since it takes an argument of type foo, it'll be found by
   *Argument-Dependent Lookup (ADL).
   *
   * There are other claims that we should specialize std::swap for our type,
   * provide an in-class swap along-side a free-function swap, etc.
   * https://stackoverflow.com/a/18605617
   * But this is all unnecessary: any proper use of swap will be through an
   *unqualified call ( outside call ),
   * and our function will be found through ADL. One function will do.
   **/

  friend void swap(Buffer &first, Buffer &second) noexcept {
    swap(first.buffer_header, second.buffer_header);
    using std::swap;
    swap(first.status, second.status);
    swap(first.data_area_size, second.data_area_size);
    swap(first.data_area, second.data_area);
    swap(first.prev_buffer_on_hash_queue, second.prev_buffer_on_hash_queue);
    swap(first.next_buffer_on_hash_queue, second.next_buffer_on_hash_queue);
    swap(first.prev_buffer_on_free_list, second.prev_buffer_on_free_list);
    swap(first.next_buffer_on_free_list, second.next_buffer_on_free_list);
  }

  /** THE BIG ZERO/THREE/FOUR/FIVE
   *
   * Read source - https://stackoverflow.com/a/3279550
   * It is important to note that Copy assignment is part of the big 3,
   * so we have already established guidelines on how to implement it.
   **/

  /** Destructor
   *
   * Only dynamic member is data_area, simply delete it
   **/

  ~Buffer() noexcept { delete[] data_area; };

  /** Copy Constructor
   *
   * deletegate to parameterized constructor
   **/

  Buffer(const Buffer &other)
      : Buffer(other.buffer_header, other.status, other.data_area_size,
               other.data_area, other.prev_buffer_on_hash_queue,
               other.next_buffer_on_hash_queue, other.prev_buffer_on_free_list,
               other.next_buffer_on_free_list) {}

  /** Move Constructor
   *
   * A possible implementation is assigning all values from other to *this and
   * then defaulting values of other to leave it in stable state.
   *
   * A better implementation however is,
   * initialise other with a default constructor, then simply swap *this and
   *other.
   * This saves code duplication, is exception safe.
   *
   **/

  Buffer(Buffer &&other) noexcept : Buffer() { swap(*this, other); }

  /** Copy Assignment
   *
   * Assignment, at its heart, is two steps:
   * tearing down the object's old state and building its new state as a copy of
   *some other object's state.
   * Basically, that's what the destructor and the copy constructor do,
   * so the first idea would be to delegate the work to them.
   * https://stackoverflow.com/a/3279616
   *
   * A naive implementation is -
   * the self copy condition, deallocating old data, then copying data members
   * This slows down, leaves data inconsisten on crash, and has code
   *duplication.
   *
   * A better implementation is the standard copy and swap idiom.
   * For this, we implemented our very swap for this structure.
   * This when done without care leads to another naive implementation -
   * creating temp copy by calling copy constructor, then swapping it with *this
   * We can do better, as the rule says -  if you're going to make a copy of
   *something in a function,
   * let the compiler do it in the parameter list. As stated in link below -
   * https://web.archive.org/web/20140113221447/http://cpp-next.com/archive/2009/08/want-speed-pass-by-value/
   *
   * The correct implementation takes the parameter by value, and then swaps it
   *with *this.
   * Here, when we enter the function we already have allocated, copied, and
   *ready to use value.
   * This ensures that *this is not changed if exception throws.
   *
   * Also note that - this call by value variant encompasses both
   *copy-assignment & move-assignment
   * so we need not make another move-assignment function
   **/

  Buffer &operator=(Buffer other) {
    swap(*this, other);
    return *this;
  }

  // overload == operator : required for hashing
  // inline bool operator==(const BufferHeader &rhs) const {
  //   return (device_number == rhs.device_number) &&
  //          (block_number == rhs.block_number);
  // }

  // inline bool operator!=(const BufferHeader &rhs) const {
  //   return !(*this == rhs);
  // }
};

int main() {

  // Buffer : verbose
  Buffer *A = new Buffer();
  //        new Buffer(BufferHeader(1, 2), 2, 4, "Hello");     // todo    debug
  cout << A->buffer_header.block_number << " " << A->data_area_size << "\n";
  // Buffer A{{1,2},2,4,"h"};
  //  Buffer B{{1,2},2,4,"hello"};
  //  B=A;
  //
  //  // Buffer : shorthand
  //  Buffer *B = new Buffer{{1, 2}, 2, 4};
  //  cout << B->buffer_header.block_number << " " << B->buffer_size << "\n";
  //
  //  // hashmap
  //  // 3rd parameter is a structure with operator() overloaded that acts as a
  //  hash
  //  // function
  //  std::unordered_map<BufferHeader, std::string, BufferHeader> my_buffer_map
  //  = {
  //      {{1, 1024}, "Hello"}, {{1024, 1024}, "wow"}};
  //  cout << my_buffer_map[{1, 1024}];
  //
  //  // set
  //  std::unordered_set<BufferHeader, BufferHeader> my_buffer_set =
  //
  //      {{1, 3}, {1, 2}, {1, 1}, {1, 3}};
  //
  //  for (auto &x : my_buffer_set)
  //  {
  //    cout << x.device_number << " " << x.block_number << "\n";
  //  }
  //
  //  BufferHeader E{1, 2};
  //  BufferHeader F{1, 2};
  //  if (E == F)
  //  {
  //    cout << "== operator overloaded";
  //  }
  return 0;
}
