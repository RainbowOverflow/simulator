/*
 * Copyright 2017 The Open University of Israel
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __LOGGING_PARSER_H__
#define __LOGGING_PARSER_H__


#include "logging_backend.h"


/**
 * Concatenate the parameters given
 * @param x the first parameter to concatenate
 * @param y the second parameter to concatenate
 * @return the concatenated parameters
 */
#define _CONCAT(x, y) x ## y
/**
 * Concatenate the parameters given
 * @param x the first parameter to concatenate
 * @param y the second parameter to concatenate
 * @return the concatenated parameters
 */
#define CONCAT(x, y) _CONCAT(x, y)


/**
 * Read bytes from the logger, while busy waiting when there are not enough data
 * @param logger the logger to read the data from
 * @param buffer the buffer to write the data to
 * @param length the number of bytes to read
 */
void logger_busy_read(Logger* logger, Byte* buffer, int length);

/**
 * Return the type of the next log in the logger
 * @param logger the logger to read the next log type from
 * @return the next log type in the logger
 */
int next_log_type(Logger* logger);


/**
 * A log of a physical page read
 */
typedef struct {
    /**
     * Member 'channel' is the channel number of the page read
     */
    unsigned int channel;
    /**
     * Member 'block' is the block number of the page read
     */
    unsigned int block;
    /**
     * Member 'page' is the page number of the page read
     */
    unsigned int page;
} PhysicalPageReadLog;
/**
 * A log of a physical page write
 */
typedef struct {
    /**
     * Member 'channel' is the channel number of the page written
     */
    unsigned int channel;
    /**
     * Member 'block' is the block number of the page written
     */
    unsigned int block;
    /**
     * Member 'page' is the page number of the page written
     */
    unsigned int page;
} PhysicalPageWriteLog;
/**
 * A log of a logical page write
 */
typedef struct {
    /**
     * Member 'channel' is the channel number of the page written
     */
    unsigned int channel;
    /**
     * Member 'block' is the block number of the page written
     */
    unsigned int block;
    /**
     * Member 'page' is the page number of the page written
     */
    unsigned int page;
} LogicalPageWriteLog;
/**
 * A log of garbage collection
 */
typedef struct {

} GarbageCollectionLog;
/*...*/


/**
 * All the logs definitions; used to easily add more log types
 * Each line should contain a call to the applier, with the structure and name of the log
 * In order to add a new log type, one must only add a new line with the log definition here
 * @param APPLIER the macro which is going to be applied to all the log types
 */
#define _LOGS_DEFINITIONS(APPLIER)  \
    APPLIER(PhysicalPageReadLog, PHYSICAL_PAGE_READ)    \
    APPLIER(PhysicalPageWriteLog, PHYSICAL_PAGE_WRITE)  \
    APPLIER(LogicalPageWriteLog, LOGICAL_PAGE_WRITE)    \
    APPLIER(GarbageCollectionLog, GARBAGE_COLLECTION)


/**
 * The enum log applier; used to create an enum of the log types' ids
 * @param structure the structure associated with the log type
 * @param name the name of the log type
 */
#define _LOGS_ENUM_APPLIER(structure, name) CONCAT(name, _LOG_UID),
/**
 * The unique ids for the different structs available for parsing
 */
enum ParsableStructures {
    /**
     * The definitions of the logs' ids of the different log types
     */
    _LOGS_DEFINITIONS(_LOGS_ENUM_APPLIER)
    /**
     * The number of different log types
     * LEAVE AS LAST ITEM!
     */
    LOG_UID_COUNT
};


/**
 * The writer log applier; used to create a writer function for the different log types
 * @param structure the structure associated with the log type
 * @param name the name of the log type
 */
#define _LOGS_WRITER_DECLARATION_APPLIER(structure, name)       \
    void CONCAT(LOG_, name)(Logger* logger, structure buffer);
/**
 * The customized LOG_X function definitions, for type-safe logging
 */
_LOGS_DEFINITIONS(_LOGS_WRITER_DECLARATION_APPLIER)


/**
 * The reader log applier; used to create a reader function for the different log types
 * @param structure the structure associated with the log type
 * @param name the name of the log type
 */
#define _LOGS_READER_DECLARATION_APPLIER(structure, name)           \
    structure CONCAT(NEXT_, CONCAT(name, _LOG))(Logger* logger);
/**
 * The customized NEXT_X_LOG definitions, for type-safe logging
 */
_LOGS_DEFINITIONS(_LOGS_READER_DECLARATION_APPLIER)


#endif
