/************************************************************************************
   Copyright (C) 2013,2018 MariaDB Corporation AB
   
   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.
   
   You should have received a copy of the GNU Library General Public
   License along with this library; if not see <http://www.gnu.org/licenses>
   or write to the Free Software Foundation, Inc., 
   51 Franklin St., Fifth Floor, Boston, MA 02110, USA
*************************************************************************************/
#ifndef _td_parse_h_
#define _td_parse_h_

#include <sqlext.h>


enum enum_madb_query_type { TDDB_QUERY_NO_RESULT= 0, /* Default type for the query types we are not interested in */ 
                            TDDB_QUERY_INSERT,
                            TDDB_QUERY_UPDATE= SQL_UPDATE,
                            TDDB_QUERY_DELETE= SQL_DELETE,
                            TDDB_QUERY_CREATE_PROC,
                            TDDB_QUERY_CREATE_FUNC,
                            TDDB_QUERY_CREATE_DEFINER,
                            TDDB_QUERY_SET,
                            TDDB_QUERY_SET_NAMES,
                            TDDB_QUERY_SELECT,
                            TDDB_QUERY_SHOW,
                            TDDB_QUERY_CALL,
                            TDDB_QUERY_ANALYZE,
                            TDDB_QUERY_EXPLAIN,
                            TDDB_QUERY_CHECK,
                            TDDB_QUERY_EXECUTE,
                            TDDB_QUERY_DESCRIBE,
                            TDDB_NOT_ATOMIC_BLOCK
};

typedef struct {
  char *QueryText;
  enum enum_madb_query_type QueryType;
  TDDB_DynArray ParamPos;
} SINGLE_QUERY;

typedef struct {
  TDDB_DynArray Tokens;
  TDDB_DynArray SubQuery; /* List of queries or batches of queries, that can be executed together at once */
  char        * Original;
  char        * allocated; /* Pointer to the allocated area. The refined query may go to the right */
  char        * RefinedText;
  size_t        RefinedLength;
  enum enum_madb_query_type QueryType;
  /* So far only falg whether we have any parameters */
  char       HasParameters;
  /* This is more for multistatements for optimization - if none of queries returns result,
     we can send them via text protocol */
  char       ReturnsResult;
  char       PoorManParsing;

  char       BatchAllowed;
  char       AnsiQuotes;
  char       NoBackslashEscape;

} TDDB_QUERY;

#define PQUERY_UPDATE_LEN(PARSED_QUERY_PTR) (PARSED_QUERY_PTR)->RefinedLength= strlen((PARSED_QUERY_PTR)->RefinedLength)
#define STMT_COUNT(PARSED_QUERY) ((PARSED_QUERY).SubQuery.elements/* + 1*/)
#define QUERY_IS_MULTISTMT(PARSED_QUERY) (STMT_COUNT(PARSED_QUERY) > 1)

int  TDDB_ResetParser(TDDB_Stmt *Stmt, char *OriginalQuery, SQLINTEGER OriginalLength);
void TDDB_DeleteSubqueries(TDDB_QUERY *Query);
void TDDB_AddSubQuery(TDDB_QUERY *Query, char *SubQueryText, enum enum_madb_query_type QueryType);

void TDDB_DeleteQuery(TDDB_QUERY *Query);
int  TDDB_ParseQuery(TDDB_QUERY *Query);

#define QUERY_DOESNT_RETURN_RESULT(query_type) ((query_type) < TDDB_QUERY_SELECT)

char *       TDDB_ParseCursorName(TDDB_QUERY *Query, unsigned int *Offset);
unsigned int TDDB_FindToken(TDDB_QUERY *Query, char *Compare);

enum enum_madb_query_type TDDB_GetQueryType(const char *Token1, const char *Token2);

const char * TDDB_FindParamPlaceholder(TDDB_Stmt *Stmt);
char *       FixIsoFormat(char * StmtString, size_t *Length);
int          ParseQuery(TDDB_QUERY *Query);
char *       StripLeadingComments(char *s, size_t *Length, BOOL OverWrite);

#endif /* _td_parse_h_ */
