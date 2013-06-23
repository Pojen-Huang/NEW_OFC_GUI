/* 
 * File:   DataModels.h
 * Author: liudanny
 *
 * Created on May 24, 2012, 3:15 PM
 */

#ifndef DATAMODELS_H
#define	DATAMODELS_H

#ifdef	__cplusplus
extern "C" {
#endif

    namespace ofcgui {

        class OFC_Model {
        public:
            QString name;
        };

        class Link_Status : public OFC_Model {
        public:
            QString *from_sw;
            int from_port;
            QString *to_sw;
            int to_port;
            QString *status;

            Link_Status(){}
            
            Link_Status(QString *_from_sw, int _from_port, QString *_to_sw,
                    int _to_port, QString *_status) {

                from_sw = _from_sw;
                from_port = _from_port;
                to_sw = _to_sw;
                to_port = _to_port;
                status = _status;
                
            }
            
            bool operator==(const Link_Status &other) const {
                 if ((this->from_sw == other.from_sw) && (this->from_port == other.from_port) &&
                         (this->to_sw == other.to_sw) && (this->to_port == other.to_port)) {
                     return true;
                 } else {
                     return false;
                 }
            } 
        };
    }


#ifdef	__cplusplus
}
#endif

#endif	/* DATAMODELS_H */

