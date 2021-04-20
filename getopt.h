#ifndef GETOPT_H
#define GETOPT_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief getopt Get option from command line arguments.
 * @param nargc  Value argc from main()
 * @param nargv  Value argv from main()
 * @param ostr   Option pattern. String with switches characters.
 *               Options that require a parameter must be followed
 *               by a colon.
 * @return
 */
int getopt(int nargc, char *nargv[], const char *ostr);

/**
 * @brief optarg Argument of option.
 */
extern const char *optarg;

#ifdef __cplusplus
}
#endif

#endif // GETOPT_H
