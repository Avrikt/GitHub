/************************************************
* Developer: Avri Kehat						    *
* Reviewed by: David Hairapetain			    *
* Date: 03.07.2023						    	*
* Description: Source file for Histo         	*
* Version: 1.0                                  *
* Status: Approved                              *
*************************************************/

#include <iostream> //cout, getline
#include <string>   //std::string
#include <vector>   //std::vector
#include <map>      //std::map

void Histo(void);

int main(void)
{
    Histo();
    return 0;
}

void Histo(void)
{
    std::vector<std::string> vec;
    std::map<std::string, int> histogram;
    std::string line;

    while(std::getline(std::cin, line) && line != ".") 
    {
        if(1 == (++histogram[line]))
        {
            vec.push_back(line);
        }
    }
    
    std::cout << std::endl << "Histogram:" << std::endl;
    size_t count = 0;
    for (size_t i = 0; i < vec.size(); ++i)
    {
        count = histogram[vec[i]];
        
        if(1 < count) 
        std::cout << count << " x ";

        std::cout << vec[i] << std::endl;
    }
}
