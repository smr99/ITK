/*=========================================================================
 *
 *  Copyright NumFOCUS
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

// Do not change NULL, null, Null in this file
// This file intentionally contains usage of legacy NULL
#include "itkGTest.h"

#include "itkSmartPointer.h"
#include "itkObject.h"

namespace
{

class Derived1 : public itk::Object
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(Derived1);

  /** Standard class type aliases. */
  using Self = Derived1;
  using Superclass = Object;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;


  /** \see LightObject::GetNameOfClass() */
  itkOverrideGetNameOfClassMacro(Derived1);

  itkNewMacro(Derived1);

  void
  Register() const override
  {
    ++m_RegisterCount;
    Superclass::Register();
  }
  unsigned int
  GetRegisterCount() const
  {
    return m_RegisterCount;
  }

protected:
  Derived1() = default;

  ~Derived1() override = default;

  mutable unsigned int m_RegisterCount{ 0 };
};


class Derived2 : public itk::Object
{
public:
  ITK_DISALLOW_COPY_AND_MOVE(Derived2);

  /** Standard class type aliases. */
  using Self = Derived2;
  using Superclass = Object;
  using Pointer = itk::SmartPointer<Self>;
  using ConstPointer = itk::SmartPointer<const Self>;


  /** \see LightObject::GetNameOfClass() */
  itkOverrideGetNameOfClassMacro(Derived2);

  itkNewMacro(Derived2);

protected:
  Derived2() = default;
  ~Derived2() override = default;
};

} // namespace

TEST(SmartPointer, EmptyAndNull)
{
  using ObjectPointer = itk::SmartPointer<itk::Object>;
  using ConstObjectPointer = itk::SmartPointer<const itk::Object>;

  ObjectPointer ptr;


  EXPECT_EQ(ptr.GetPointer(), nullptr);
  EXPECT_TRUE(ptr.IsNull());
  EXPECT_FALSE(ptr.IsNotNull());
  EXPECT_FALSE(static_cast<bool>(ptr));

  ConstObjectPointer cptr;

  EXPECT_EQ(cptr.GetPointer(), nullptr);
  EXPECT_TRUE(cptr.IsNull());
  EXPECT_FALSE(cptr.IsNotNull());
  EXPECT_FALSE(static_cast<bool>(cptr));


  ptr = ObjectPointer(nullptr);
  EXPECT_TRUE(ptr.IsNull());

  cptr = ObjectPointer(nullptr);
  EXPECT_TRUE(cptr.IsNull());

  ptr = itk::Object::New();
  EXPECT_TRUE(ptr.IsNotNull());
  EXPECT_TRUE(static_cast<bool>(ptr));

  cptr = itk::Object::New();
  EXPECT_TRUE(cptr.IsNotNull());
  EXPECT_TRUE(static_cast<bool>(cptr));

  ptr = nullptr;
  EXPECT_TRUE(ptr.IsNull());

  cptr = nullptr;
  EXPECT_TRUE(cptr.IsNull());

  ITK_CLANG_PRAGMA_PUSH
  ITK_CLANG_SUPPRESS_Wzero_as_null_pointer_constant

  // NOLINTNEXTLINE(modernize-use-nullptr)
  ptr = NULL; // Do not change NULL, null, Null in this file. This file intentionally contains usage of legacy NULL
  EXPECT_TRUE(ptr.IsNull());

  // NOLINTNEXTLINE(modernize-use-nullptr)
  cptr = NULL; // Do not change NULL, null, Null in this file. This file intentionally contains usage of legacy NULL
  ITK_CLANG_PRAGMA_POP

  EXPECT_TRUE(cptr.IsNull());


  EXPECT_TRUE(cptr == nullptr);
  EXPECT_TRUE(nullptr == cptr);

  // This does not work with VS14 2015
  //  EXPECT_TRUE( ptr == 0 );
  //  EXPECT_TRUE( 0 == ptr );

  // Exercise pointer assignment
  auto                         p1 = Derived2::New();
  const Derived2::Pointer      p2 = p1;
  const Derived2::ConstPointer cp1 = p1;
  const Derived2::ConstPointer cp2(p1);
  const Derived2::ConstPointer cp3{ p1 };
}


TEST(SmartPointer, Converting)
{

  using BasePointer = itk::SmartPointer<itk::Object>;
  using ConstBasePointer = itk::SmartPointer<const itk::Object>;

  using Derived1Pointer = itk::SmartPointer<Derived1>;
  using ConstDerived1Pointer = itk::SmartPointer<const Derived1>;

  const Derived1Pointer d1ptr = Derived1::New();
  EXPECT_TRUE(d1ptr.IsNotNull());
  EXPECT_FALSE(d1ptr.IsNull());
  EXPECT_TRUE(static_cast<bool>(d1ptr));
  EXPECT_FALSE(d1ptr == nullptr);
  EXPECT_TRUE(d1ptr != nullptr);

  const ConstDerived1Pointer cd1ptr(d1ptr);
  EXPECT_TRUE(cd1ptr.GetPointer() == d1ptr.GetPointer());
  EXPECT_TRUE(cd1ptr == d1ptr);

  BasePointer            ptr(d1ptr);
  const ConstBasePointer cptr1(d1ptr);
  const ConstBasePointer cptr2(cd1ptr);

  ptr = d1ptr;


  const itk::Object * rcptr1 = ptr;
  EXPECT_TRUE(rcptr1 == ptr);

  rcptr1 = cd1ptr;
  EXPECT_TRUE(rcptr1 == ptr.GetPointer());

  const Derived1 * rcd1ptr = d1ptr;
  EXPECT_TRUE(rcd1ptr == d1ptr);

  rcd1ptr = cd1ptr;
  EXPECT_TRUE(rcd1ptr == cd1ptr);

  // is_convertible<From,To>
  static_assert(std::is_convertible_v<Derived1Pointer, BasePointer>, "conversion check");
  static_assert(std::is_convertible_v<Derived1Pointer, ConstBasePointer>, "conversion check");

  static_assert(!std::is_convertible_v<ConstDerived1Pointer, Derived1Pointer>, "conversion check");

  static_assert(!std::is_convertible_v<Derived1Pointer, Derived2::Pointer>, "conversion check");
  static_assert(!std::is_convertible_v<Derived1Pointer, Derived2::ConstPointer>, "conversion check");
}


TEST(SmartPointer, ConvertingRegisterCount)
{

  using Derived1Pointer = itk::SmartPointer<Derived1>;
  using ConstDerived1Pointer = itk::SmartPointer<const Derived1>;

  using BasePointer = itk::SmartPointer<itk::Object>;

  // Copy constructor to const
  {
    const Derived1Pointer d1ptr = Derived1::New();
    EXPECT_EQ(1, d1ptr->GetRegisterCount());

    const ConstDerived1Pointer cd1ptr = d1ptr;
    EXPECT_EQ(2, d1ptr->GetRegisterCount());
    EXPECT_EQ(2, cd1ptr->GetRegisterCount());
  }

  // Copy constructor to base
  {
    const Derived1Pointer d1ptr = Derived1::New();
    EXPECT_EQ(1, d1ptr->GetRegisterCount());

    const BasePointer bptr = d1ptr;
    EXPECT_EQ(2, d1ptr->GetRegisterCount());
    EXPECT_EQ(2, static_cast<const Derived1 *>(bptr.GetPointer())->GetRegisterCount());
  }

  // Assignment operator
  {
    const Derived1Pointer d1ptr = Derived1::New();
    EXPECT_EQ(1, d1ptr->GetRegisterCount());

    Derived1Pointer d1ptr2;

    d1ptr2 = d1ptr;
    EXPECT_EQ(2, d1ptr->GetRegisterCount());
    EXPECT_EQ(2, d1ptr2->GetRegisterCount());
  }

  // Assignment to const pointer
  {
    const Derived1Pointer d1ptr = Derived1::New();
    EXPECT_EQ(1, d1ptr->GetRegisterCount());

    ConstDerived1Pointer cd1ptr;

    cd1ptr = d1ptr;
    EXPECT_EQ(2, d1ptr->GetRegisterCount());
    EXPECT_EQ(2, cd1ptr->GetRegisterCount());
  }

  // Assignment to base pointer
  {
    const Derived1Pointer d1ptr = Derived1::New();
    EXPECT_EQ(1, d1ptr->GetRegisterCount());

    BasePointer bptr;

    bptr = d1ptr;
    EXPECT_EQ(2, d1ptr->GetRegisterCount());
    EXPECT_EQ(2, static_cast<const Derived1 *>(bptr.GetPointer())->GetRegisterCount());
  }

  // Assignment to raw pointer
  {
    const Derived1Pointer d1ptr = Derived1::New();
    EXPECT_EQ(1, d1ptr->GetRegisterCount());

    Derived1 * rptr = d1ptr;
    EXPECT_EQ(1, d1ptr->GetRegisterCount());
    EXPECT_TRUE(rptr != nullptr);
  }

  // Assignment to raw const pointer
  {
    const Derived1Pointer d1ptr = Derived1::New();
    EXPECT_EQ(1, d1ptr->GetRegisterCount());

    const Derived1 * rptr = d1ptr;
    EXPECT_EQ(1, d1ptr->GetRegisterCount());
    EXPECT_TRUE(rptr != nullptr);
  }

  // Move constructor
  {
    Derived1Pointer d1ptr = Derived1::New();
    EXPECT_EQ(1, d1ptr->GetRegisterCount());

    const Derived1Pointer d1ptr2(std::move(d1ptr));
    EXPECT_EQ(1, d1ptr2->GetRegisterCount());
    EXPECT_TRUE(d1ptr.IsNull());
  }

  // Move constructor to const
  {
    Derived1Pointer d1ptr = Derived1::New();
    EXPECT_EQ(1, d1ptr->GetRegisterCount());

    const ConstDerived1Pointer cd1ptr(std::move(d1ptr));
    EXPECT_EQ(1, cd1ptr->GetRegisterCount());
    EXPECT_TRUE(d1ptr.IsNull());
  }

  // Move constructor to base
  {
    Derived1Pointer d1ptr = Derived1::New();
    EXPECT_EQ(1, d1ptr->GetRegisterCount());

    const BasePointer bptr(std::move(d1ptr));
    EXPECT_EQ(1, static_cast<const Derived1 *>(bptr.GetPointer())->GetRegisterCount());
    EXPECT_TRUE(d1ptr.IsNull());
  }

  // Move assignment
  {
    Derived1Pointer d1ptr = Derived1::New();
    EXPECT_EQ(1, d1ptr->GetRegisterCount());

    Derived1Pointer d1ptr2;
    d1ptr2 = std::move(d1ptr);
    EXPECT_EQ(1, d1ptr2->GetRegisterCount());
    EXPECT_TRUE(d1ptr.IsNull());
  }

  // Move assignment to const
  {
    Derived1Pointer d1ptr = Derived1::New();
    EXPECT_EQ(1, d1ptr->GetRegisterCount());

    ConstDerived1Pointer cd1ptr;
    cd1ptr = std::move(d1ptr);

    EXPECT_EQ(1, cd1ptr->GetRegisterCount());
    EXPECT_TRUE(d1ptr.IsNull());
  }

  // Move assignment to base
  {
    Derived1Pointer d1ptr = Derived1::New();
    EXPECT_EQ(1, d1ptr->GetRegisterCount());

    BasePointer bptr;

    bptr = std::move(d1ptr);
    EXPECT_EQ(1, static_cast<const Derived1 *>(bptr.GetPointer())->GetRegisterCount());
    EXPECT_TRUE(d1ptr.IsNull());
  }
}


// Tests that `smartPointer.get()` is equivalent to `smartPointer.GetPointer()`.
TEST(SmartPointer, GetIsEquivalentToGetPointer)
{
  const auto check = [](auto && smartPointer) { EXPECT_EQ(smartPointer.get(), smartPointer.GetPointer()); };

  check(itk::Object::New());
  check(itk::Object::Pointer{});
  check(itk::Object::ConstPointer{});
  check(itk::SmartPointer<itk::Object>{});
}
