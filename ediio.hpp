#ifndef EDIIO_HPP
#define EDIIO_HPP

#include "adifio.hpp"

namespace EDIIO {

    void write_qso_record(std::ostream &oss, const ADIFIO::ObjectMap_t &adif);

} // end namespace EDIIO //

#endif // EDIIO_HPP
