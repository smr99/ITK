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
#include "itkStatisticsImageFilter.h"
#include "itkImageTestHelper.h"

class StatisticsImageFilterFixture : public ::testing::Test
{
protected:
  using ImageType = itk::Image<double, 2>;
  using FilterType = itk::StatisticsImageFilter<ImageType>;

  ImageType::Pointer m_sumtest_image;
  double             m_sumtest_sum;

  StatisticsImageFilterFixture()
  {
    m_sumtest_image = CreateImage<ImageType>({ 1000, 1000 }, 1e-20);
    m_sumtest_image->SetPixel({ 0, 0 }, 1);
    m_sumtest_sum = 1 + 1e-14 - 1e-20; // 1 + 999,999 * 1e-20
  }

  FilterType::Pointer
  RunFilter(const ImageType * image)
  {
    auto filter = FilterType::New();
    filter->SetInput(image);
    filter->Update();
    return filter;
  }
};


// Demonstration code: test fails using double, but succeeds using CompensatedSummation
TEST(StatisticsImageFilter, test0)
{
  // double sum = 1.0;
  itk::CompensatedSummation<double> sum = 1;
  for (int i = 0; i < 1000000; ++i)
  {
    sum += 1e-20;
  }

  EXPECT_EQ((double)sum, 1 + 1e-14);
}

TEST_F(StatisticsImageFilterFixture, sum)
{
  auto filter = RunFilter(m_sumtest_image);
  EXPECT_DOUBLE_EQ(filter->GetSum(), m_sumtest_sum);
}

TEST_F(StatisticsImageFilterFixture, mean)
{
  auto filter = RunFilter(m_sumtest_image);
  EXPECT_DOUBLE_EQ(filter->GetMean(), m_sumtest_sum / 1000000);
}
