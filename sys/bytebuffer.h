#ifndef __SYS_BYTEBUFFER_H__
#define __SYS_BYTEBUFFER_H__

#include "sys.h"

namespace util {
class ByteBuffer
{
public:
class out_of_range : public std::exception
{
public:
out_of_range(uint32_t p, uint32_t l, uint32_t s);
virtual ~out_of_range() throw() {}

virtual const char* what() const throw() ;
private:
static const uint32_t MAX_ERROR_MSG_LEN = 256;
char errmsg_[MAX_ERROR_MSG_LEN];
};
public:
explicit ByteBuffer() ;
explicit ByteBuffer(uint32_t size);
explicit ByteBuffer(const ByteBuffer& rhs);
ByteBuffer(const char* data, uint32_t offset, uint32_t size);
ByteBuffer & assign(const char* data, uint32_t offset, uint32_t size);
ByteBuffer & wrap(char* data, uint32_t offset, uint32_t size);
ByteBuffer & operator=(const ByteBuffer& rhs);
virtual ~ByteBuffer();

public:
// generic put & get
template <typename T> ByteBuffer & put (const T & e) throw (out_of_range);
template <typename T> ByteBuffer & get (T & e) throw (out_of_range);
template <typename T> ByteBuffer & put (const std::vector<T> & v) throw (out_of_range) ;
template <typename T> ByteBuffer & get (std::vector<T> & v) throw (out_of_range);
template <typename T> ByteBuffer & operator<<(const T &e) throw (out_of_range) { return put(e); }
template <typename T> ByteBuffer & operator>>(T &e) throw (out_of_range) { return get(e); }

template <typename T> const ByteBuffer & peek (T & e) const throw (out_of_range);
template <typename T> T get () throw (out_of_range);

// specialize put & get with std::string
ByteBuffer & put(const std::string & e) throw (out_of_range) { return putString(e);  }
ByteBuffer & get(std::string & e) throw (out_of_range) { return getString(e); }


ByteBuffer & putString(const std::string & v) throw (out_of_range);
ByteBuffer & getString(std::string & v) throw (out_of_range);

// get data_ directly..
virtual ByteBuffer & put(const char* src, uint32_t offset, uint32_t size)  throw (out_of_range);
virtual ByteBuffer & get(char* dst, uint32_t offset, uint32_t size) throw (out_of_range);
// relative get method, from index of data_
ByteBuffer & get(int index, char* dst, uint32_t offset, uint32_t size) throw (out_of_range);
// fetch data_ directly, use them very carefully
ByteBuffer & getRef(int index, const char* &dst, uint32_t size) throw (out_of_range);
const ByteBuffer & rawData(int index, const char* &dst, uint32_t size) const throw (out_of_range) ;

template <typename T> ByteBuffer & getRef(int index, T* &dst) throw (out_of_range);
template <typename T> ByteBuffer & getRef(int index, const T* &dst) throw (out_of_range);

public:
void      reset();
void      reset(uint32_t size);
uint32_t  position(uint32_t p) throw (ByteBuffer::out_of_range);
uint32_t  position() const { return position_; }
uint32_t  size() const { return size_; }
int32_t   remaining() const { return size_ - position_; }

protected:
char* allocate(uint32_t size) const;
void  free();
ByteBuffer & copy(const char* data, uint32_t offset, uint32_t length);

protected:
char* data_;
uint32_t size_;
uint32_t position_;
bool  own_;
};

template <typename T>
ByteBuffer & ByteBuffer::put(const T & e)
throw (ByteBuffer::out_of_range)
{
if (position_ + sizeof(T) > size_)
throw out_of_range(position_, sizeof(T), size_);
memcpy(data_ + position_, &e, sizeof(T));
position_ += sizeof(T);
return *this;
}

template <typename T>
ByteBuffer & ByteBuffer::get(T & e)
throw (ByteBuffer::out_of_range)
{
if (position_ + sizeof(T) > size_)
throw out_of_range(position_, sizeof(T), size_);
peek(e);
position_ += sizeof(T);
return *this;
}

template <typename T>
T ByteBuffer::get()
throw (ByteBuffer::out_of_range)
{
T e;
get(e);
return e;
}


template <typename T>
const ByteBuffer & ByteBuffer::peek(T & e) const
throw (ByteBuffer::out_of_range)
{
if (position_ + sizeof(T) > size_)
throw out_of_range(position_, sizeof(T), size_);
memcpy(&e, data_ + position_, sizeof(T));
return *this;
}

template <typename T>
ByteBuffer & ByteBuffer::put(const std::vector<T> & v)
throw (ByteBuffer::out_of_range)
{
put(v.size());
for (uint32_t i = 0; i < v.size(); ++i)
{
put(v[i]);
}
return *this;
}

template <typename T>
ByteBuffer & ByteBuffer::get(std::vector<T> & v)
throw (ByteBuffer::out_of_range)
{
uint32_t size;
get(size);
T e;
for (uint32_t i = 0; i < size; ++i)
{
get(e);
v.push_back(e);
}
return *this;
}

template <typename T>
ByteBuffer & ByteBuffer::getRef(int index, T* &dst)
throw (out_of_range)
{
const char* ref = 0;
getRef(index, ref, sizeof(T));
dst = (T*) ref;
return *this;
}

template <typename T>
ByteBuffer & ByteBuffer::getRef(int index, const T* &dst)
throw (out_of_range)
{
const char* ref = 0;
getRef(index, ref, sizeof(T));
dst = (const T*) ref;
return *this;
}

} // end namespace tfs

#endif //__BYTEBUFFER_H__












