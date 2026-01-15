#pragma once

#define REQUIRED_CONFIG_FIELDS "Required:\n\t\"compiler\"" \
                               "\n\t\"cpp\"" \
                               "\n\t\"build\"" \
                               "\n\t\"source\"\n"

#define NO_CONFIG_ERR          "Please create a " \
                               "coup_config.json with " \
                               "the required fields\n\t" \
                               "see --required\n"

#define MISSING_FIELDS_ERR     "coup_config.json is missing " \
                               "required fields\n" REQUIRED

#define INVALID_FIELD_ERR(x)   "Invalid field in " \
                               "coup_config.json: "#x \
                               "\n"

#define INVALID_ENTRY_ERR(k,v) "Invalid entry in " \
                               "coup_config.json: "#v \
                               " (field: "#k")\n"
                                
