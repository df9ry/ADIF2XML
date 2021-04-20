#include "adif2xml.hpp"
#include "adif2xml_version.hpp"
#include "getopt.h"

#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include "date/date.h"
#include "adifio.hpp"
#include "ediio.hpp"

using namespace std;
using namespace date;
using namespace ADIFIO;
using namespace EDIIO;

static void help(const char *name)
{
    cerr << "Usage: " << name << endl
         << "\t-i <filename> ............ Read ADIF file from <filename> (default: STDIN)" << endl
         << "\t-o <filename> ............ Write XMLfile to <filename> (defalt: STDOUT)" << endl
         << "\t-v ....................... Write verbose messages to STDERR" << endl
         << "\t-h ....................... Print help (this message)" << endl;
}

static void headerCB(const ObjectMap_t &/*tokens*/, void */*userData*/)
{
#if 0
    ostream &oss{*static_cast<ostream*>(userData)};

    oss << "Header:" << endl;
    for_each(tokens.begin(), tokens.end(), [&](auto &pair) {
        const string &key{pair.first};
        const Object &val{pair.second};
        oss << key << " = " << val.to_string() << endl;
    }); // end for_each //
#endif
}

static void recordCB(const ObjectMap_t &tokens, void *userData)
{
    ostream &oss{*static_cast<ostream*>(userData)};

#define EDI
#ifdef EDI
    write_qso_record(oss, tokens);
#else
    oss << "Record:" << endl;
    for_each(tokens.begin(), tokens.end(), [&](auto &pair) {
        const string &key{pair.first};
        const Object &val{pair.second};
        oss << key << " = " << val.to_string() << endl;
    }); // end for_each //
#endif
}

int main(int argc, char* argv[]) {
    int option{0}, verbose{0};
    string inputfile{""}, outputfile{""};

    // Get options:
    while ((option = getopt(argc, argv, "hi:o:v")) >= 0) {
        switch (option) {
        case 'h':
            help(argv[0]);
            return EXIT_SUCCESS;
        case 'i':
            if (inputfile.length() > 0) {
                help(argv[0]);
                return EXIT_FAILURE;
            }
            inputfile = optarg;
            break;
        case 'o':
            if (outputfile.length() > 0) {
                help(argv[0]);
                return EXIT_FAILURE;
            }
            outputfile = optarg;
            break;
        case 'v':
            ++verbose;
            break;
        default:
            help(argv[0]);
            return EXIT_FAILURE;
        } // end switch //
    } // end while //

    if (verbose)
        cerr << "This is " APP_NAME << " version " << APP_VERSION << endl
             << "  Copyright (c) " << APP_COPYRIGHT << " see <" << APP_WEBSITE << ">" << endl;

    try {
        istream *input{(inputfile.length() > 0) ? new ifstream(inputfile) : &cin};
        ostream *output{(outputfile.length() > 0) ? new ofstream(outputfile) : &cout};

        if (inputfile.length() > 0) {
            if (!filesystem::exists(inputfile))
                throw runtime_error("File not found: \"" + inputfile + "\"");
            if (input->bad())
                throw runtime_error("Bad inputfile: \"" + inputfile + "\"");
        }
        if (verbose)
            cerr << "Reading ADIF from "
                 << ((inputfile.length() > 0) ? inputfile : "<STDIN>")
                 << endl;

        if (outputfile.length() > 0) {
            if (output->bad())
                throw runtime_error("Bad outputfile: \"" + outputfile + "\"");
        }
        if (verbose)
            cerr << "Writing XML to "
                 << ((outputfile.length() > 0) ? outputfile : "<STDOUT>")
                 << endl;

        ADIFIO::Reader r;
        r.readADI(*input, recordCB, output, headerCB, output);
    }
    catch (const exception& ex) {
        cerr << "ERROR: " << ex.what() << endl;
        return EXIT_FAILURE;
    }
    catch (...) {
        cerr << "ERROR: Unidentified failure!" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
