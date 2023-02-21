#include <cstdio>
#include <stdexcept>
#include <cstring>

const int MAX_DEC = 18;
const int STD_DEC = 6;
const int MAX_TOKEN_TAG = 8;
const int MAX_POOL_TAG = 24;
const int MAX_ADD = 44;


bool constexpr isBigNum(int _dec) {
  if (_dec > MAX_DEC) throw std::runtime_error { "[!] NUM too big." };
  return _dec > STD_DEC;
}

struct Token {
  Token (const char *_tag, const char *_add, int _dec) : dec { _dec } { 
    if (strlen(_tag) >= MAX_TOKEN_TAG) throw std::runtime_error { "[!] TAG length too long." };
    if (strlen(_add) >= MAX_ADD) throw std::runtime_error { "[!] ADD length too long." };
    tag = new char[MAX_TOKEN_TAG];
    add = new char[MAX_ADD];
    std::strncpy(tag, _tag, MAX_TOKEN_TAG);
    std::strncpy(add, _add, MAX_ADD);
    isBig = isBigNum(dec);
  }

  ~Token () { 
    delete [] tag;
    delete [] add;
  }

  Token(const Token& src) noexcept : tag{ new char[MAX_TOKEN_TAG] }, add{ new char[MAX_ADD] }, dec{ src.dec }, isBig{ src.isBig } {
    std::strncpy(tag, src.tag, MAX_TOKEN_TAG);
    std::strncpy(add, src.add, MAX_ADD);   
  }

  Token(Token&& src) noexcept : tag{ src.tag }, add{ src.add }, dec{ src.dec }, isBig{ src.isBig } {
    src.tag = nullptr;
    src.add = nullptr;
    src.dec = 0;
    src.isBig = 0;
  }

  Token &operator=(const Token &src) {
    if (this == &src) return *this;
    const auto tag_ = new char [MAX_TOKEN_TAG];
    const auto add_ = new char [MAX_ADD];
    
    delete [] tag;
    delete [] add;

    tag = tag_;
    add = add_;
    dec = src.dec;
    isBig = src.isBig;
    std::strncpy(tag, src.tag, MAX_TOKEN_TAG);
    std::strncpy(add, src.add, MAX_ADD);
    return *this;
 }

  bool p () { 
    return isBig; 
  }
  
  char *tag;
  char *add;
  int dec;

  private:
    bool isBig;
};

enum class PoolType {
  UNSET,
  CLASSIC,
  STABLE,
  CONSTANT,
  HYBRID
};

struct Pool {
  Pool (const char *_tag, const char *_add, PoolType _type) : type{ _type } { 
    if (strlen(_tag) >= MAX_POOL_TAG) throw std::runtime_error { "[!] TAG length too long." };
    if (strlen(_add) >= MAX_ADD) throw std::runtime_error { "[!] ADD length too long." };
    tag = new char[MAX_POOL_TAG];
    add = new char[MAX_ADD];
    std::strncpy(tag, _tag, MAX_POOL_TAG);
    std::strncpy(add, _add, MAX_ADD);
  }

  Pool () { 
    delete [] tag;
    delete [] add;
  }

  Pool(const Pool& src) noexcept : 
    tag{ new char[MAX_POOL_TAG] }, 
    add{ new char[MAX_ADD] }, 
    dec{ src.dec }, 
    type{ src.type },
    isBig{ src.isBig}
  {
    std::strncpy(tag, src.tag, MAX_POOL_TAG);
    std::strncpy(add, src.add, MAX_ADD);   
  }

  Pool(Pool&& src) noexcept : tag{ src.tag }, add{ src.add }, dec{ src.dec }, type{ src.type }, isBig{ src.isBig } {
    src.tag = nullptr;
    src.add = nullptr;
    src.dec = 0;
    src.type = PoolType::UNSET;
    src.isBig = 0;
  }

  Pool &operator=(const Pool &src) {
    if (this == &src) return *this;
    const auto tag_ = new char [MAX_POOL_TAG];
    const auto add_ = new char [MAX_ADD];
    
    delete [] tag;
    delete [] add;

    tag = tag_;
    add = add_;
    dec = src.dec;
    type = src.type;
    isBig = src.isBig;
    std::strncpy(tag, src.tag, MAX_POOL_TAG);
    std::strncpy(add, src.add, MAX_ADD);
    return *this;
 }

  void set_tag(const char _tag) {
    if (strlen(_tag) >= MAX_POOL_TAG) throw std::runtime_error { "[!] TAG length too long." };
    tag = (*char) _tag;
  }

  void set_add(const char _add) {
    if (strlen(_tag) >= MAX_ADD) throw std::runtime_error { "[!] TAG length too long." };
    add = (*char) _add;
  }

  void set_dec(int _dec) {
    isBig = isBigNum;
    dec = _dec;
  }
  
  bool p () { 
    return isBig; 
  }

  char *tag;
  char *add;
  int dec { 18 };
  PoolType type;
  
  private:
    bool isBig { true };
};
