/*-----------------------------------------------------------------------------
 *
 * parser_dl.c
 *			  
 *		
 *		AUTHOR: lord_pretzel
 *
 *		
 *
 *-----------------------------------------------------------------------------
 */

#include "common.h"
#include "log/logger.h"
#include "mem_manager/mem_mgr.h"
#include "parser/parser.h"
#include "parser/parse_internal_dl.h"
#include "parser/parser_dl.h"
#include "dl_parser.tab.h"
#include "instrumentation/timing_instrumentation.h"

static Node *parseInternaldl (void);

Node *
parseStreamdl (FILE *stream)
{
    dlin = stream;

    return parseInternaldl();
}

Node *
parseFromStringdl (char *input)
{
    INFO_LOG("parse SQL:\n%s", input);
    dlSetupStringInput(input);

    return parseInternaldl();
}

static Node *
parseInternaldl (void) //TODO make copyObject work first
{
    Node *result;
    START_TIMER("module - parser");

    NEW_AND_ACQUIRE_MEMCONTEXT("PARSER_CONTEXT");

    // parse
    int rc = dlparse();
    if (rc)
    {
        FATAL_LOG("parse error!");
        return NULL;
    }

    STOP_TIMER("module - parser");

    DEBUG_LOG("datalog model generated by parser is:\n%s\n\n%s",
            nodeToString(dlParseResult),
            beatify(nodeToString(dlParseResult)));

    // create copy of parse result in parent context
    FREE_MEM_CONTEXT_AND_RETURN_COPY(Node,dlParseResult);
}

