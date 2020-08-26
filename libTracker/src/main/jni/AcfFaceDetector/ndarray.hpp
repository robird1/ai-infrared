/**--------------------------------------------------
* ULSEE-ACF-Detector
*
* Copyright (c) 2016
* Written by Jiaolong Xu
*--------------------------------------------------*/

#ifndef _ND_ARRAY_HPP_
#define _ND_ARRAY_HPP_
#include <string.h>
#include <vector>

using namespace std;

namespace ulsee {

template<typename T>
class NDMat {
public:
    // Constructor
    NDMat();
    
    NDMat(uint32_t imH, uint32_t imW,
            uint32_t imC);
    
    NDMat(uint32_t imH, uint32_t imW,
            uint32_t imC, T val);

    NDMat(const NDMat<T> &other);
    
    // Destructor
    ~NDMat(); 

    /* Save data to txt
     * @param filename: txt file name
     */
    void saveTXT(const char* filename); 
    
    /* Load data from txt
     * @param filename: txt file name
     */
    void loadFromTXT(const char* filename);
    
    // Return total number of elements
    uint32_t size() const { return w * c * h; }

    // Copy assigment
    NDMat<T>& operator=(const NDMat<T>& other);
   
    /* Resize ndarray
     * @param newH: new height
     * @param newW: new width
     */
    NDMat<T> resize(const uint32_t newH, const uint32_t newW);

    /* Crop to a sub-ndarray
     *
     * @param imH: crop height
     * @param imW: crop width
     */
    NDMat<T> crop(const int imH, const int imW) const;

    T* data;    // data
    uint32_t w; // width
    uint32_t h; // height
    uint32_t c; // channels
};
}
#endif // _ND_ARRAY_HPP_
