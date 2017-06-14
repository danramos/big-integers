#include <dojo-test-tools.hpp>
#include <limits>
#include <string>
#include <algorithm>

class BigInteger
{
public:
    BigInteger(const std::string& x) : _number(x.rbegin(), x.rend())
    {
        std::transform(_number.begin(), _number.end(), _number.begin(),
                       [](char i) { return i - '0'; });
    }

    std::string toString() const
    {
        std::string x(_number.size(), 0);
        std::transform(_number.rbegin(), _number.rend(), x.begin(),
                       [](int i) { return i + '0';});
        return x;
    }

    BigInteger operator+(const BigInteger& that) const
    {
        const auto& shorter_number = (_number.size() < that._number.size()) ? _number : that._number;
        const auto& longer_number = (_number.size() >= that._number.size()) ? _number : that._number;

        std::string result;
        int carry = 0;
        for (auto i = 0u; i < longer_number.size(); i++) {
            const auto sum = ((i < shorter_number.size()) ? 
                                 _number[i] + that._number[i] : longer_number[i]) + carry;

            const auto real_sum = sum % 10;
            carry = sum / 10;
            result = std::to_string(real_sum) + result;
        }

        if (carry) {
            result = std::to_string(carry) + result;
        }

        return BigInteger(result);
    }

    BigInteger operator*(const BigInteger& that) const
    {
        BigInteger final_result{"0"};

        for (auto j = 0u; j < that._number.size(); j++) {
            std::string result = "";
            auto product =0;
            auto carry = 0;

            for (auto i = 0u; i < _number.size(); i++) {
                    product = _number[i] * that._number[j] + carry;
                    carry = product / 10;
                    product %= 10;
                    result = std::to_string(product) + result;
            }

            if (carry) {
                result = std::to_string(carry) + result;
            }

            const auto partial_product = BigInteger(result + std::string(j, '0'));
            final_result = final_result + partial_product;
        }
        

        return final_result;
    }

private:
    //std::string _number;
    std::vector<int> _number;
};

namespace helpers
{
    std::string getSumAsString(const std::string& a, const std::string& b)
    {
        return (BigInteger{a} + BigInteger{b}).toString();
    }

    std::string getProductAsString(const std::string& a, const std::string& b)
    {
        return (BigInteger{a} * BigInteger{b}).toString();
    }

}

TEST_CASE(GetStringRepresentationOfInteger)
{
    ASSERT_EQUAL("1", BigInteger{"1"}.toString());
    ASSERT_EQUAL("2", BigInteger{"2"}.toString());
    ASSERT_EQUAL("3", BigInteger{"3"}.toString());
    ASSERT_EQUAL("4", BigInteger{"4"}.toString());
}

TEST_CASE(AddSingleDigitsWithoutCarries)
{
    ASSERT_EQUAL("3", helpers::getSumAsString("1", "2"));
    ASSERT_EQUAL("4", helpers::getSumAsString("2", "2"));
    ASSERT_EQUAL("6", helpers::getSumAsString("3", "3"));
    ASSERT_EQUAL("9", helpers::getSumAsString("4", "5"));
}

TEST_CASE(DoubleDigitAdditionWithoutCarries)
{
    ASSERT_EQUAL("22", helpers::getSumAsString("12", "10"));
    ASSERT_EQUAL("35", helpers::getSumAsString("22", "13"));
    ASSERT_EQUAL("44", helpers::getSumAsString("30", "14"));
    ASSERT_EQUAL("55", helpers::getSumAsString("30", "25"));
}

TEST_CASE(ThreeDigitAdditionWithoutCarries)
{
    ASSERT_EQUAL("222", helpers::getSumAsString("120", "102"));
    ASSERT_EQUAL("357", helpers::getSumAsString("222", "135"));
    ASSERT_EQUAL("444", helpers::getSumAsString("303", "141"));
    ASSERT_EQUAL("555", helpers::getSumAsString("302", "253"));
}

TEST_CASE(DoubleAndSingleDigitWithoutCarry)
{
    ASSERT_EQUAL("69", helpers::getSumAsString("62", "7"));
    ASSERT_EQUAL("69", helpers::getSumAsString("7", "62"));
    ASSERT_EQUAL("101", helpers::getSumAsString("1", "100"));
    ASSERT_EQUAL("1001", helpers::getSumAsString("1000", "1"));
    ASSERT_EQUAL("10001", helpers::getSumAsString("1", "10000"));
}

TEST_CASE(DoubleDigitsWithCarry)
{
    ASSERT_EQUAL("43", helpers::getSumAsString("17", "26"));
    ASSERT_EQUAL("55", helpers::getSumAsString("19", "36"));
}

TEST_CASE(SingleDigitsWithCarry)
{
    ASSERT_EQUAL("10", helpers::getSumAsString("9", "1"));
    ASSERT_EQUAL("15", helpers::getSumAsString("7", "8"));
}

TEST_CASE(UnequalDigitsWithCarry)
{
    ASSERT_EQUAL("10000000", helpers::getSumAsString("9999999", "1"));
    ASSERT_EQUAL("29000001", helpers::getSumAsString("28999909", "92"));
}

TEST_CASE(SampleAddition) {
    ASSERT_EQUAL("9938365271", helpers::getSumAsString("3567660388", "6370704883"));
    ASSERT_EQUAL("15540176289", helpers::getSumAsString("5566557220", "9973619069"));
    ASSERT_EQUAL("21910881172", helpers::getSumAsString("15540176289", "6370704883"));
    ASSERT_EQUAL("21979643824", helpers::getSumAsString("21910881172", "68762652"));
    ASSERT_EQUAL("68219945372916622428512047648316131870653354719718", helpers::getSumAsString("21910881172", "68219945372916622428512047648316131870631443838546"));
    ASSERT_EQUAL("299958199141494661895726693483088492735044589847425", helpers::getSumAsString("268219945372916622428512047648316131870653354719718", "31738253768578039467214645834772360864391235127707"));
}

TEST_CASE(SingleDigitMultiplication)
{
    ASSERT_EQUAL("4", helpers::getProductAsString("2", "2"));
    ASSERT_EQUAL("56", helpers::getProductAsString("8", "7"));
}

TEST_CASE(MultipleAndSingleDigitFactors)
{
    ASSERT_EQUAL("96", helpers::getProductAsString("16", "6"));
    ASSERT_EQUAL("891", helpers::getProductAsString("99", "9"));
    ASSERT_EQUAL("8212887", helpers::getProductAsString("912543", "9"));
    ASSERT_EQUAL("48632327934", helpers::getProductAsString("8105387989", "6"));
}

TEST_CASE(MultipleDigitFactors)
{
    ASSERT_EQUAL("2208", helpers::getProductAsString("69", "32"));
    ASSERT_EQUAL("656934565", helpers::getProductAsString("8105", "81053"));
    ASSERT_EQUAL("602021926449811", helpers::getProductAsString("7427509487", "81053"));
}

TEST_CASE(FinalTestCase)
{
    const std::string num_a = "8401663228665897217032553420030090782623648570885777549839013059950919411293858400815765259629276238764039864681310825138103373823255323016255508548038562600498804650616517051421957625610762062290499275314044159183691090110873555808893954951923560199407325";
    const std::string num_b = "4943081937844944349097127011207330422667112259072541231362927451862154881071594979982381824951667864814910785163973860246061199292088961039731324176839524052101361917910562370952022549116527545652297299298784702856823913308820849712943545530128109513552689";
    const std::string expected_product = "41530109753474435009490556652655646807981825043359532731448926965423400785913325575640352364557182229094244896904589806455613734594638808319146495150950929557664631552283563358389585661075803887378460652503544223480789246194643224101737297143176053237057019167098011472053370034680282187105828417530658167789408403981198624606636634849452356862870821465621366851135347055222672415873369793742740318938978149782232434834492740036186702499491879433249288963012402212984054596332425748773746931803185857432960046925";

    ASSERT_EQUAL(expected_product, helpers::getProductAsString(num_a, num_b));

}

