#ifndef PARAMSLOADER_H
#define PARAMSLOADER_H

#define MAKE_STR(ARG) #ARG


//macro to read parameters
//if it can't find the parameter it will apply the dafault which is hard coded in each .h
#define BEGIN_READ_SETTINGS(FILE,VAR_GROUP,VERBOSE) { \
                                            QSettings settings(FILE,QSettings::IniFormat); \
                                            int verbose = VERBOSE;\
                                            settings.beginGroup(#VAR_GROUP);

#define READ_SETTING(VAR_GROUP,VAR,CONVERT_TO) if (settings.contains(#VAR)){ \
                                                   VAR_GROUP.VAR = settings.value(#VAR,VAR_GROUP.VAR).to##CONVERT_TO(); \
                                                   if (verbose) {qDebug() << MAKE_STR(VAR_GROUP.VAR) << "=" << VAR_GROUP.VAR;} \
                                               } \

//the one below will report any setting it can't find which I think is too much
#if 0
#define READ_SETTING(VAR_GROUP,VAR,CONVERT_TO) if (settings.contains(#VAR)){ \
                                                   VAR_GROUP.VAR = settings.value(#VAR,VAR_GROUP.VAR).to##CONVERT_TO(); \
                                                   if (verbose) {qDebug() << MAKE_STR(VAR_GROUP.VAR) << "=" << VAR_GROUP.VAR;} \
                                               }else{ \
                                                    qDebug() << "cannot find" << MAKE_STR(VAR_GROUP.VAR) << "in" << settings.group() << "so set to" << VAR_GROUP.VAR ; \
                                               }
#endif
#define END_READ_SETTINGS settings.endGroup();}

#endif //PARAMSLOADER_H