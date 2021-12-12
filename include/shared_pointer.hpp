// Copyright 2021 Petrova Kseniya <ksyushki5@yandex.ru>

#ifndef INCLUDE_SHARED_POINTER_HPP_
#define INCLUDE_SHARED_POINTER_HPP_
#include <iostream>
#include <atomic>
#include <stdexcept>
#include <utility>
template <typename T>
class shared_pointer {
 public:
  shared_pointer(); //конструктор по умолчанию
  explicit shared_pointer(T* ptr); //конструктор (принимает сырой указатель)
  shared_pointer(const shared_pointer& r); //конструктор копирования
  shared_pointer(shared_pointer&& r); //конструктор перемещения
  ~shared_pointer(); //деструктор

  auto get() const -> T*;//возвращает адрес контролируемого ресурса

  auto operator=(const shared_pointer& r) -> shared_pointer&;//для копирования
  auto operator=(shared_pointer&& r) -> shared_pointer&;//для перемещения

  operator bool() const;//указывает ли указатель на объект
  //oператор косвенного обращения, возвращает *get()
  auto operator*() const -> T&;
  auto operator->() const -> T*;//oператор выбора, возвращает get()

  void reset(); //освобождает ресурс
  void reset(T* ptr); //заменяет ресурс
  void swap(shared_pointer& r);//меняет местами два объекта shared_pointer
  //возвращает количество объектов, которые ссылаются на
  // тот же управляемый объект
  auto use_count() const -> size_t;

 private:
  T* _ptr;
  std::atomic_uint *_count_ptr;
};

//------------------------------Конструкторы------------------------------------
template <typename T>
shared_pointer<T>::shared_pointer()
    :_ptr(nullptr),
      _count_ptr(nullptr)
{}

template <typename T>
shared_pointer<T>::shared_pointer(T *ptr)
    :_ptr(ptr),
      _count_ptr(new std::atomic<unsigned int>)
{*_count_ptr = 1;}

template <typename T>shared_pointer<T>::
    shared_pointer(const shared_pointer<T> &r)
    :_ptr(r._ptr)
{
  this->_count_ptr = r._count_ptr;
  ++*_count_ptr;
}

template <typename T>
shared_pointer<T>::shared_pointer(shared_pointer<T> &&r)
    :_ptr(r._ptr),
      _count_ptr(r._count_ptr)
{
  ++*_count_ptr;
}
//---------------------------------Деструктор-----------------------------------
template <typename T>
shared_pointer<T>::~shared_pointer<T>()
{
  if (_count_ptr) {
    if (*_count_ptr == 1) {
      delete _count_ptr;
      _count_ptr = nullptr;
      _ptr = nullptr;
    } else {
      --(*_count_ptr);
    }
  }
}
//----------------------------------get-----------------------------------------
template <typename T>
auto shared_pointer<T>::get() const -> T*
{
  return _ptr;
}
//------------------------Перегрузка операторов = ------------------------------
template <typename T>
auto shared_pointer<T>::operator=(const shared_pointer& r) -> shared_pointer&
{
  _ptr = r.get();
  if (_count_ptr != nullptr){
    --(*_count_ptr);
  }
  _count_ptr = r._count_ptr;
  ++(*_count_ptr);
  return get();
}

template <typename T>
auto shared_pointer<T>::operator=(shared_pointer&& r) -> shared_pointer&
{
  _ptr = std::move(r.object);
  if (this->_count_ptr != nullptr){
    --(*_count_ptr);
  }
  _count_ptr = std::move(r._count_ptr);
  ++(*_count_ptr);
  return get();
}
//--------------------------Перегрузка операторов-------------------------------
template <typename T>
shared_pointer<T>::operator bool() const
{
  if (_ptr != nullptr)
    return true;
  else
    return false;
}

template <typename T>
auto shared_pointer<T>::operator*() const -> T & {
  if (this->_ptr == nullptr){
    throw std::runtime_error{"Empty pointer!"};
  }
  return *get();
}

template <typename T>
auto shared_pointer<T>::operator->() const -> T * {
  return get();
}

template <typename T>
void shared_pointer<T>::reset()
{
  if (_count_ptr != nullptr) {
    _ptr = nullptr;
    if (*_count_ptr != 1) {
      --(*_count_ptr);
    }else{
      delete _count_ptr;
    }
    _count_ptr = nullptr;
  }
}
//------------------------------------------------------------------------------
template <typename T>
void shared_pointer<T>::reset(T *ptr) {
  if (_count_ptr != nullptr) {
    --(*_count_ptr);
  }
  _count_ptr = new std::atomic<uint>;
  *_count_ptr = 1;
  _ptr = ptr;
}

template <typename T>
void shared_pointer<T>::swap(shared_pointer<T> &r) {
  shared_pointer tmp(r);
  r._ptr = _ptr;
  r._count_ptr = _count_ptr;
  _ptr = std::move(tmp._ptr);
  _count_ptr = std::move(tmp._count_ptr);
}

template <typename T>
auto shared_pointer<T>::use_count() const -> size_t {
  return *_count_ptr;
}
#endif // INCLUDE_SHARED_POINTER_HPP_
