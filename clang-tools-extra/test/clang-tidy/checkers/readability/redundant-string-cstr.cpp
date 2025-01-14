// RUN: %check_clang_tidy %s readability-redundant-string-cstr %t

typedef unsigned __INT16_TYPE__ char16;
typedef unsigned __INT32_TYPE__ char32;
typedef __SIZE_TYPE__ size;

namespace std {
template <typename T>
class allocator {};
template <typename T>
class char_traits {};
template <typename C, typename T, typename A>
struct basic_string {
  typedef basic_string<C, T, A> _Type;
  basic_string();
  basic_string(const C *p, const A &a = A());

  ~basic_string();

  const C *c_str() const;
  const C *data() const;

  _Type& append(const C *s);
  _Type& append(const C *s, size n);
  _Type& assign(const C *s);
  _Type& assign(const C *s, size n);

  int compare(const _Type&) const;
  int compare(const C* s) const;
  int compare(size pos, size len, const _Type&) const;
  int compare(size pos, size len, const C* s) const;

  size find(const _Type& str, size pos = 0) const;
  size find(const C* s, size pos = 0) const;
  size find(const C* s, size pos, size n) const;

  _Type& insert(size pos, const _Type& str);
  _Type& insert(size pos, const C* s);
  _Type& insert(size pos, const C* s, size n);

  _Type& operator+=(const _Type& str);
  _Type& operator+=(const C* s);
  _Type& operator=(const _Type& str);
  _Type& operator=(const C* s);
};

typedef basic_string<char, std::char_traits<char>, std::allocator<char>> string;
typedef basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t>> wstring;
typedef basic_string<char16, std::char_traits<char16>, std::allocator<char16>> u16string;
typedef basic_string<char32, std::char_traits<char32>, std::allocator<char32>> u32string;

template <typename C, typename T>
struct basic_string_view {
  basic_string_view(const C* s);
};
typedef basic_string_view<char, std::char_traits<char>> string_view;
typedef basic_string_view<wchar_t, std::char_traits<wchar_t>> wstring_view;
typedef basic_string_view<char16, std::char_traits<char16>> u16string_view;
typedef basic_string_view<char32, std::char_traits<char32>> u32string_view;
}

std::string operator+(const std::string&, const std::string&);
std::string operator+(const std::string&, const char*);
std::string operator+(const char*, const std::string&);

bool operator==(const std::string&, const std::string&);
bool operator==(const std::string&, const char*);
bool operator==(const char*, const std::string&);

template <typename T>
struct iterator {
  T *operator->();
};

namespace llvm {
struct StringRef {
  StringRef(const char *p);
  StringRef(const std::string &);
};
}

// Tests for std::string.

void f1(const std::string &s) {
  f1(s.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}f1(s);{{$}}
  f1(s.data());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'data' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}f1(s);{{$}}
}
void f2(const llvm::StringRef r) {
  std::string s;
  f2(s.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}std::string s;{{$}}
  // CHECK-FIXES-NEXT: {{^  }}f2(s);{{$}}
}
void f3(const llvm::StringRef &r) {
  std::string s;
  f3(s.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}std::string s;{{$}}
  // CHECK-FIXES-NEXT: {{^  }}f3(s);{{$}}
}
void f4(const std::string &s) {
  const std::string* ptr = &s;
  f1(ptr->c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}f1(*ptr);{{$}}

  std::string tmp;
  tmp = ptr->c_str();
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}tmp = *ptr;{{$}}
}
void f5(const std::string &s) {
  std::string tmp;
  tmp.append(s.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}tmp.append(s);{{$}}
  tmp.assign(s.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}tmp.assign(s);{{$}}

  if (tmp.compare(s.c_str()) == 0) return;
  // CHECK-MESSAGES: :[[@LINE-1]]:19: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}if (tmp.compare(s) == 0) return;{{$}}

  if (tmp.compare(1, 2, s.c_str()) == 0) return;
  // CHECK-MESSAGES: :[[@LINE-1]]:25: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}if (tmp.compare(1, 2, s) == 0) return;{{$}}

  if (tmp.find(s.c_str()) == 0) return;
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}if (tmp.find(s) == 0) return;{{$}}

  if (tmp.find(s.c_str(), 2) == 0) return;
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}if (tmp.find(s, 2) == 0) return;{{$}}

  if (tmp.find(s.c_str(), 2) == 0) return;
  // CHECK-MESSAGES: :[[@LINE-1]]:16: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}if (tmp.find(s, 2) == 0) return;{{$}}

  tmp.insert(1, s.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}tmp.insert(1, s);{{$}}

  tmp = s.c_str();
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}tmp = s;{{$}}

  tmp += s.c_str();
  // CHECK-MESSAGES: :[[@LINE-1]]:10: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}tmp += s;{{$}}

  if (tmp == s.c_str()) return;
  // CHECK-MESSAGES: :[[@LINE-1]]:14: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}if (tmp == s) return;{{$}}

  tmp = s + s.c_str();
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}tmp = s + s;{{$}}

  tmp = s.c_str() + s;
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: redundant call {{.*}}
  // CHECK-FIXES: {{^  }}tmp = s + s;{{$}}
}
void f6(const std::string &s) {
  std::string tmp;
  tmp.append(s.c_str(), 2);
  tmp.assign(s.c_str(), 2);

  if (tmp.compare(s) == 0) return;
  if (tmp.compare(1, 2, s) == 0) return;

  tmp = s;
  tmp += s;

  if (tmp == s)
    return;

  tmp = s + s;

  if (tmp.find(s.c_str(), 2, 4) == 0) return;

  tmp.insert(1, s);
  tmp.insert(1, s.c_str(), 2);
}
void f7(std::string_view sv) {
  std::string s;
  f7(s.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}f7(s);{{$}}
  f7(s.data());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'data' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}f7(s);{{$}}
}

// Tests for std::wstring.

void g1(const std::wstring &s) {
  g1(s.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}g1(s);{{$}}
}
void g2(std::wstring_view sv) {
  std::wstring s;
  g2(s.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}g2(s);{{$}}
  g2(s.data());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'data' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}g2(s);{{$}}
}

// Tests for std::u16string.

void h1(const std::u16string &s) {
  h1(s.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}h1(s);{{$}}
}
void h2(std::u16string_view sv) {
  std::u16string s;
  h2(s.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}h2(s);{{$}}
  h2(s.data());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'data' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}h2(s);{{$}}
}

// Tests for std::u32string.

void k1(const std::u32string &s) {
  k1(s.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}k1(s);{{$}}
}
void k2(std::u32string_view sv) {
  std::u32string s;
  k2(s.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}k2(s);{{$}}
  k2(s.data());
  // CHECK-MESSAGES: :[[@LINE-1]]:6: warning: redundant call to 'data' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}k2(s);{{$}}
}

// Tests on similar classes that aren't good candidates for this checker.

struct NotAString {
  NotAString();
  NotAString(const NotAString&);
  const char *c_str() const;
};

void dummy(const char*) {}

void invalid(const NotAString &s) {
  dummy(s.c_str());
}

// Test for rvalue std::string.
void m1(std::string&&) {
  std::string s;

  m1(s.c_str());

  void (*m1p1)(std::string&&);
  m1p1 = m1;
  m1p1(s.c_str());

  using m1tp = void (*)(std::string &&);
  m1tp m1p2 = m1;
  m1p2(s.c_str());  
}

// Test for iterator
void it(iterator<std::string> i)
{
  std::string tmp;
  tmp = i->c_str();
  // CHECK-MESSAGES: :[[@LINE-1]]:9: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}tmp = *i;{{$}}
}

namespace PR45286 {
struct Foo {
  void func(const std::string &) {}
  void func2(std::string &&) {}
};

void bar() {
  std::string Str{"aaa"};
  Foo Foo;
  Foo.func(Str.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:12: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}Foo.func(Str);{{$}}

  // Ensure it doesn't transform Binding to r values
  Foo.func2(Str.c_str());

  // Ensure its not confused by parens
  Foo.func((Str.c_str()));
  // CHECK-MESSAGES: :[[@LINE-1]]:13: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}Foo.func((Str));{{$}}
  Foo.func2((Str.c_str()));
}
} // namespace PR45286

namespace fmt {
  inline namespace v8 {
    template<typename ...Args>
    void print(const char *, Args &&...);
    template<typename ...Args>
    std::string format(const char *, Args &&...);
  }
}

namespace notfmt {
  inline namespace v8 {
    template<typename ...Args>
    void print(const char *, Args &&...);
    template<typename ...Args>
    std::string format(const char *, Args &&...);
  }
}

void fmt_print(const std::string &s1, const std::string &s2, const std::string &s3) {
  fmt::print("One:{}\n", s1.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:26: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}fmt::print("One:{}\n", s1);

  fmt::print("One:{} Two:{} Three:{}\n", s1.c_str(), s2, s3.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:42: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-MESSAGES: :[[@LINE-2]]:58: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}fmt::print("One:{} Two:{} Three:{}\n", s1, s2, s3);
}

// There's no c_str() call here, so it shouldn't be touched
void fmt_print_no_cstr(const std::string &s1, const std::string &s2) {
    fmt::print("One: {}, Two: {}\n", s1, s2);
}

// This isn't fmt::print, so it shouldn't be fixed.
void not_fmt_print(const std::string &s1) {
    notfmt::print("One: {}\n", s1.c_str());
}

void fmt_format(const std::string &s1, const std::string &s2, const std::string &s3) {
  auto r1 = fmt::format("One:{}\n", s1.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:37: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}auto r1 = fmt::format("One:{}\n", s1);

  auto r2 = fmt::format("One:{} Two:{} Three:{}\n", s1.c_str(), s2, s3.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:53: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-MESSAGES: :[[@LINE-2]]:69: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}auto r2 = fmt::format("One:{} Two:{} Three:{}\n", s1, s2, s3);
}

// There's are c_str() calls here, so it shouldn't be touched
void fmt_format_no_cstr(const std::string &s1, const std::string &s2) {
    fmt::format("One: {}, Two: {}\n", s1, s2);
}

// This is not fmt::format, so it shouldn't be fixed
std::string not_fmt_format(const std::string &s1) {
    return notfmt::format("One: {}\n", s1.c_str());
}

class BaseTrace {
public:
  template <typename... Args>
  void operator()(const char *fmt, Args &&...args) {
  }

  template <typename... Args>
  void Trace(const char *fmt, Args &&...args) {
  }

  template <typename... Args>
  void F(const char *fmt, Args &&...args) {
  }
};

class DerivedTrace : public BaseTrace {};

class DoubleDerivedTrace : public DerivedTrace {};

typedef DerivedTrace TypedefDerivedTrace;

class NullTrace {
public:
  template <typename... Args>
  void operator()(const char *fmt, Args &&...args) {
  }

  template <typename... Args>
  void Trace(const char *fmt, Args &&...args) {
  }

  template <typename... Args>
  void F(const char *fmt, Args &&...args) {
  }
};

class NullDerivedTrace : public NullTrace {};

void trace1(const std::string &s1, const std::string &s2, const std::string &s3) {
  BaseTrace TRACE;

  TRACE("%s\n", s1.c_str(), s2, s3.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:17: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-MESSAGES: :[[@LINE-2]]:33: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}TRACE("%s\n", s1, s2, s3);

  DerivedTrace TRACE2;
  TRACE2("%d %s\n", 42, s1.c_str(), s2.c_str(), s3);
  // CHECK-MESSAGES: :[[@LINE-1]]:25: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-MESSAGES: :[[@LINE-2]]:37: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}TRACE2("%d %s\n", 42, s1, s2, s3);

  DoubleDerivedTrace TRACED;
  TRACED("%d %s\n", 42, s1.c_str(), s2, s3.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:25: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-MESSAGES: :[[@LINE-2]]:41: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}TRACED("%d %s\n", 42, s1, s2, s3);

  TypedefDerivedTrace TRACET;
  TRACET("%d %s\n", 42, s1.c_str(), s2, s3.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:25: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-MESSAGES: :[[@LINE-2]]:41: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}TRACET("%d %s\n", 42, s1, s2, s3);
}

void trace2(const std::string &s1, const std::string &s2) {
  NullTrace TRACE3;

  TRACE3("%s\n", s1.c_str(), s2.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:18: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-MESSAGES: :[[@LINE-2]]:30: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}TRACE3("%s\n", s1, s2);

  NullDerivedTrace TRACE4;
  TRACE4("%d %s\n", 42, s1.c_str(), s2.c_str());
  // CHECK-MESSAGES: :[[@LINE-1]]:25: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-MESSAGES: :[[@LINE-2]]:37: warning: redundant call to 'c_str' [readability-redundant-string-cstr]
  // CHECK-FIXES: {{^  }}TRACE4("%d %s\n", 42, s1, s2);
}
