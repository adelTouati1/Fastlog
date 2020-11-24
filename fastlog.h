#ifndef __FASTLOG_H
#define __FASTLOG_H

/* The maximum number of logs that our ring buffer will hold at any one time.*/
#define MAX_LOG_ENTRY 500

/*The maximum length of an individual log entry.*/
#define MAX_MSG_LENGTH 80

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    INFO,
    WARNING,
    ERROR
} LEVEL;

/**
 * Initialize the buffer for use. 
 * We must call this function before calling any other function 
 * to ensure we are all ready. 
 */
void fastlog_init(void);

/**
 * Add an entry into the logger
 * We can assume that text is a properly terminated string. 
 * However we can NOT assume that entry is less than MAX_MSG_LENGTH,
 * so we must be careful when copying our data.
 */
void fastlog_write(LEVEL lvl, char *text);


/**
 * Print all valid logs to stderr using the following format:
 * [pid]-[timestamp]-[LEVEL]-<message_text>"
 * 
 * NOTE: Logs should be printed from oldest to newest
 */
void fastlog_dump(void);

/**
 * Write the contents of the fastlog buffer to a binary file
 * named "fastlog.core.<pid>" where <pid> is the current 
 * process id. This binary data is the array of entry structs and 
 * NOT string data. 
 * 
 * NOTE:  This is implemented in Fastlog Library (part 3) */
// void fastlog_core_dump(void);

/**
 * Load the contents of the specified core file into the fastlog
 * buffer, overwriting the existing contents, beginning at
 * index position 0.  Once this binary core has been loaded,
 * the fastlog_dump() function can be used to display the logs
 * in the console. 
 * 
 * NOTE:  This is implemented in Fastlog Library (part 3) */
// void fastlog_core_load(const char *filename);


#ifdef __cplusplus
}
#endif

#endif /* __FASTLOG_H */
