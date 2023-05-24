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

#include "itkGTest.h"
#include "itkLabelStatisticsImageFilter.h"
#include "itkImageTestHelper.h"

class LabelStatisticsImageFilterFixture : public ::testing::Test
{
protected:
  using ImageType = itk::Image<double, 2>;
  using LabelImageType = itk::Image<short, 2>;
  using FilterType = itk::LabelStatisticsImageFilter<ImageType, LabelImageType>;

  using LabelType = FilterType::LabelPixelType;
  using RealType = FilterType::RealType;

  ImageType::Pointer      m_Image;
  LabelImageType::Pointer m_LabelImage;
  FilterType::Pointer     m_Filter;

  ImageType::Pointer m_sumtest_image;
  RealType           m_sumtest_sum;

  LabelStatisticsImageFilterFixture()
  {
    m_sumtest_image = CreateImage<ImageType>({ 1000, 1000 }, 1e-20);
    m_sumtest_image->SetPixel({ 0, 0 }, 1);
    m_sumtest_sum = 1 + 1e-14 - 1e-20; // 1 + 999,999 * 1e-20

    m_LabelImage = CreateImage<LabelImageType>({ 1000, 1000 }, 1);
  }


  FilterType::Pointer
  RunFilter(const ImageType * image, const LabelImageType * labelImage)
  {
    auto filter = FilterType::New();
    filter->SetInput(image);
    filter->SetLabelInput(labelImage);
    filter->Update();
    return filter;
  }

  void
  CheckStats(LabelType label, RealType min, RealType max)
  {
    m_Filter->Update();
    ASSERT_DOUBLE_EQ(m_Filter->GetMinimum(label), min);
    ASSERT_DOUBLE_EQ(m_Filter->GetMaximum(label), max);
  }
};


TEST_F(LabelStatisticsImageFilterFixture, sum)
{
  auto filter = RunFilter(m_sumtest_image, m_LabelImage);
  EXPECT_DOUBLE_EQ(filter->GetSum(1), m_sumtest_sum);
}

TEST_F(LabelStatisticsImageFilterFixture, mean)
{
  auto filter = RunFilter(m_sumtest_image, m_LabelImage);
  EXPECT_DOUBLE_EQ(filter->GetMean(1), m_sumtest_sum / 1000000);
}
