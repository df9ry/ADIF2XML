#include "ediio.hpp"

using namespace std;
using namespace ADIFIO;

namespace EDIIO {

    static string find(const ObjectMap_t &adif, const string &key, bool f_throw)
    {
        ObjectMap_t::const_iterator iter{adif.find(key)};
        if (iter == adif.end()) {
            if (f_throw)
                throw runtime_error("Not found: \"" + key + "\"");
            else
                return "";
        } else {
            return iter->second.to_string();
        }
    }

    static string removeNonDigit(const string &s)
    {
        ostringstream oss;
        for (char ch: s) {
            if (isdigit(ch))
                oss.put(ch);
        } // end for //
        return oss.str();
    }

    static int modeCode(const string &s, bool f_throw)
    {
        if (s == "SSB")  return 1;
        if (s == "CW")   return 2;
        if (s == "AM")   return 5;
        if (s == "FM")   return 6;
        if (s == "RTTY") return 7;
        if (s == "SSTV") return 8;
        if (s == "ATV")  return 9;
        if (f_throw)
            throw runtime_error("Invalid mode: \"" + s + "\"");
        else
            return 0;
    }

    void write_qso_record(ostream &oss, const ObjectMap_t &adif)
    {
        oss << removeNonDigit(find(adif, "QSO_DATE", true)).substr(2) << ";"
            << removeNonDigit(find(adif, "TIME_ON", true)).substr(0, 4) << ";"
            << find(adif, "CALL", true) << ";"
            << modeCode(find(adif, "MODE", true), false) << ";"
            << find(adif, "RST_SENT", true) << ";"
            << find(adif, "STX_STRING", true).substr(0, 3) << ";"
            << find(adif, "RST_RCVD", true) << ";"
            << find(adif, "SRX_STRING", true).substr(0, 3) << ";;"
            << find(adif, "SRX_STRING", true).substr(3) << ";0"
            << ";;;;" << endl;
    }

} // end namespace EDIIO //
