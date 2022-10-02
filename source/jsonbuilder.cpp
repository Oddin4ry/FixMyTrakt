#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#ifndef CLASS_LOGGER
#include "ClassLogger.cpp"
#define CLASS_LOGGER
#endif

#include <jsoncpp/json/json.h>

class JsonBuilder {
    enum {
        VALUE_TYPE_NONE,
        VALUE_TYPE_NUMBER,
        VALUE_TYPE_STRING
    };
    public:
        void parseString(const char *pJson){
            Json::Reader lReader;
            if(!lReader.parse(pJson, gRoot)){
                gLogger.log("Parse error");
            }
        }

        bool isValueValue(Json::Value pValue, const char *pCompare){
            if(pValue.isString()){
                return pValue.asString() == pCompare;
            }else if(pValue.isNumeric()){
                std::stringstream lSS;
                lSS << pValue.asLargestUInt();
                return lSS.str() == pCompare;
            }
            return false;
        }

        const char *displayValueValue(Json::Value pValue){
            if(pValue.isString()){
                return pValue.asCString();
            }else if(pValue.isNumeric()){
                std::stringstream lSS;
                lSS << pValue.asLargestUInt();
            }else if(pValue.isArray()){
                std::stringstream lSS;
                lSS << "Array of " << pValue.size();
            }else if(pValue.isObject()){
                return "Object";
            }
            return "Not sure";
        }

        std::string getFormatted(){
            std::stringstream lSS;
            Json::StyledStreamWriter lWriter;
            lWriter.write(lSS, gRoot);
            return lSS.str();
        }

        void addJsonVector(const char *pName, std::vector<Json::Value> pVector){
            Json::Value lArray;
            //gRoot.resize(pVector.size());
            for(Json::Value lItem : pVector){
                lArray.append(lItem);
            }
            //lArray.setComment(pName);
            Json::Value lNew;
            lNew[pName] = lArray;
            gRoot = lNew;
            //lNewParent << lArray;
     //       gRoot.append(lArray);
        }

        std::vector<Json::Value> getAllChilden(const std::string pKey){
            std::vector<Json::Value> lResults;
            for (int i = 0; i < gRoot.size(); i++){
                lResults.push_back(gRoot[i][pKey]);
                //for(auto const& id :gRoot[i].getMemberNames()){
                  //  if()
                //}
                /*if(gRoot[i].asString()==pKey){
                    gLogger.log("Found ", gRoot[i].asString());
                    lResults.push_back(gRoot[i]);
                }*/
            }
            return lResults;
        }

        Json::Value newBinaryValue(const char *pName, char unsigned *pBinary, int pLength){
            return newValue(pName, base64_encode(pBinary, pLength).c_str());
        }

        Json::Value newValue(const char *pName, const char *pValue){
            Json::Value lValue;
            lValue[pName] = pValue;
            gRoot.append(lValue);
            return lValue;
        }

        void setRoot(const char *pName){
            gRoot = pName;
        }

        Json::Value newValue(Json::Value pParent, const char *pName, const char *pValue){
            Json::Value lValue;
            lValue[pName] = pValue;
            pParent.append(lValue);
            return lValue;
        }

        std::string getStringValue(const char *pName){
            Json::Value lValue = gRoot[pName];
            if(lValue!=0){
                return lValue.asString();
            }
            return "";
        }
        bool isEmpty(){
            return gRoot.empty();
        }

        int getSize(){
            return gRoot.size();
        }

        long long getIntegerValue(const char *pName){
            Json::Value lValue = gRoot[pName];
            if(lValue!=0){
                return lValue.asLargestInt();
            }
            return 0;
        }
        Json::Value gRoot;        
    private:

        Logger gLogger = Logger("jsonbuilder");


        const unsigned char base64_table[65] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        /**
        * base64_encode - Base64 encode
        * @src: Data to be encoded
        * @len: Length of the data to be encoded
        * @out_len: Pointer to output length variable, or %NULL if not used
        * Returns: Allocated buffer of out_len bytes of encoded data,
        * or empty string on failure
        */

        const int B64index[256] = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
        0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
        56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
        7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
        0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 };

        std::string b64decode(const void* data, const size_t len)
        {
            unsigned char* p = (unsigned char*)data;
            int pad = len > 0 && (len % 4 || p[len - 1] == '=');
            const size_t L = ((len + 3) / 4 - pad) * 4;
            std::string str(L / 4 * 3 + pad, '\0');

            for (size_t i = 0, j = 0; i < L; i += 4)
            {
                int n = B64index[p[i]] << 18 | B64index[p[i + 1]] << 12 | B64index[p[i + 2]] << 6 | B64index[p[i + 3]];
                str[j++] = n >> 16;
                str[j++] = n >> 8 & 0xFF;
                str[j++] = n & 0xFF;
            }
            if (pad)
            {
                int n = B64index[p[L]] << 18 | B64index[p[L + 1]] << 12;
                str[str.size() - 1] = n >> 16;

                if (len > L + 2 && p[L + 2] != '=')
                {
                    n |= B64index[p[L + 2]] << 6;
                    str.push_back(n >> 8 & 0xFF);
                }
            }
            return str;
        }       
        std::string base64_encode(const unsigned char *src, size_t len)
        {
            unsigned char *out, *pos;
            const unsigned char *end, *in;

            size_t olen;

            olen = 4*((len + 2) / 3); /* 3-byte blocks to 4-byte */

            if (olen < len)
                return std::string(); /* integer overflow */

            std::string outStr;
            outStr.resize(olen);
            out = (unsigned char*)&outStr[0];

            end = src + len;
            in = src;
            pos = out;
            while (end - in >= 3) {
                *pos++ = base64_table[in[0] >> 2];
                *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
                *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
                *pos++ = base64_table[in[2] & 0x3f];
                in += 3;
            }

            if (end - in) {
                *pos++ = base64_table[in[0] >> 2];
                if (end - in == 1) {
                    *pos++ = base64_table[(in[0] & 0x03) << 4];
                    *pos++ = '=';
                }
                else {
                    *pos++ = base64_table[((in[0] & 0x03) << 4) |
                        (in[1] >> 4)];
                    *pos++ = base64_table[(in[1] & 0x0f) << 2];
                }
                *pos++ = '=';
            }

            return outStr;
        }        
};