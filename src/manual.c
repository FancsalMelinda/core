/* 
   Copyright (C) 2008 - Mark Burgess

   This file is part of Cfengine 3 - written and maintained by Mark Burgess.
 
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 3, or (at your option) any
   later version. 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA

*/

/*****************************************************************************/
/*                                                                           */
/* File: manual.c                                                            */
/*                                                                           */
/*****************************************************************************/

#include "cf3.defs.h"
#include "cf3.extern.h"

extern char BUILD_DIR[CF_BUFSIZE];
extern char MANDIR[CF_BUFSIZE];

void TexinfoHeader(FILE *fout);
void TexinfoFooter(FILE *fout);
void TexinfoBodyParts(FILE *fout,struct BodySyntax *bs,char *context);
void TexinfoSubBodyParts(FILE *fout,struct BodySyntax *bs);
void TexinfoShowRange(FILE *fout,char *s,enum cfdatatype type);
void IncludeManualFile(FILE *fout,char *filename);
void TexinfoPromiseTypesFor(FILE *fout,struct SubTypeSyntax *st);

/*****************************************************************************/

void TexinfoManual(char *mandir)

{ char filename[CF_BUFSIZE];
  struct SubTypeSyntax *st;
  char *thischapter = NULL;
  FILE *fout;
  int i,j;

snprintf(filename,CF_BUFSIZE-1,"%scf3-Reference.texinfo",BUILD_DIR);

if ((fout = fopen(filename,"w")) == NULL)
   {
   CfOut(cf_error,"fopen","Unable to open %s for writing\n",filename);
   return;
   }

TexinfoHeader(fout);

/* General background */

fprintf(fout,"@c *****************************************************\n");
fprintf(fout,"@c * CHAPTER \n");
fprintf(fout,"@c *****************************************************\n");

fprintf(fout,"@node Basics\n@chapter Cfengine %s Basics\n\n",VERSION);
IncludeManualFile(fout,"reference_basics.texinfo");

/* Control promises */

fprintf(fout,"@c *****************************************************\n");
fprintf(fout,"@c * CHAPTER \n");
fprintf(fout,"@c *****************************************************\n");

fprintf(fout,"@node Control Promises\n@chapter Control promises\n\n");
IncludeManualFile(fout,"reference_control_intro.texinfo");

for (i = 0; CF_ALL_BODIES[i].btype != NULL; i++)
   {
   fprintf(fout,"@node control %s\n@section @code{%s} control promises\n\n",CF_ALL_BODIES[i].btype,CF_ALL_BODIES[i].btype);
   TexinfoBodyParts(fout,CF_ALL_BODIES[i].bs,CF_ALL_BODIES[i].btype);
   snprintf(filename,CF_BUFSIZE-1,"control_%s_example.texinfo",CF_ALL_BODIES[i].btype);
   IncludeManualFile(fout,filename);
   snprintf(filename,CF_BUFSIZE-1,"control_%s_notes.texinfo",CF_ALL_BODIES[i].btype);
   IncludeManualFile(fout,filename);
   }

/* Components */

for (i = 0; i < CF3_MODULES; i++)
   {
   st = (CF_ALL_SUBTYPES[i]);

   if (st == CF_COMMON_SUBTYPES || st == CF_EXEC_SUBTYPES || st == CF_REMACCESS_SUBTYPES || st == CF_KNOWLEDGE_SUBTYPES)
      {
      Verbose("Dealing with chapter / bundle type %s\n",st->btype);
      fprintf(fout,"@c *****************************************************\n");
      fprintf(fout,"@c * CHAPTER \n");
      fprintf(fout,"@c *****************************************************\n");
      
      if (strcmp(st->btype,"*") == 0)
         {
         fprintf(fout,"@node Bundles for common\n@chapter Bundles of @code{common}\n\n");
         }
      else
         {
         fprintf(fout,"@node Bundles for %s\n@chapter Bundles of @code{%s}\n\n",st->btype,st->btype);
         }
      }
      
   TexinfoPromiseTypesFor(fout,st);
   snprintf(filename,CF_BUFSIZE-1,"bundletype_%s_example.texinfo",st->btype);
   IncludeManualFile(fout,filename);
   snprintf(filename,CF_BUFSIZE-1,"bundletype_%s_notes.texinfo",st->btype);
   IncludeManualFile(fout,filename);
   }

/* Other sub-bundles */


TexinfoFooter(fout);

fclose(fout);
}

/*****************************************************************************/
/* Level                                                                     */
/*****************************************************************************/

void TexinfoHeader(FILE *fout)
{
 fprintf(fout,
         "\\input texinfo @c -*-texinfo-*-\n"
         "@c *********************************************************************\n"
         "@c\n"
         "@c  This is an AUTO_GENERATED TEXINFO file. Do not submit patches against it.\n"
         "@c  Refer to the the component .texinfo files instead when patching docs.\n"
         "@c\n"
         "@c ***********************************************************************\n"
         "@c %%** start of header\n"
         "@setfilename cf3-reference.info\n"
         "@settitle Cfengine reference manual (version %s)\n"
         "@setchapternewpage odd\n"
         "@c %%** end of header\n"
         "@titlepage\n"
         "@title Cfengine Reference Manual\n"
         "@subtitle Auto-generated cfengine knowledge\n"
         "@subtitle for version %s\n"
         "@author cfengine.com\n"
         "@c @smallbook\n"
         "@fonttextsize 10\n"
         "@page\n"
         "@vskip 0pt plus 1filll\n"
         "Copyright @copyright{} 2008 Cfengine AS\n"
         "@end titlepage\n"
         "@c *************************** File begins here ************************\n"
         "@ifinfo\n"
         "@dircategory Cfengine Training\n"
         "@direntry\n"
         "* cfengine Modularization:\n"
         "                        Cfengine is a language based framework\n"
         "                        designed for configuring and maintaining\n"
         "                        Unix-like operating systems attached\n"
         "                        to a TCP/IP network.\n"
         "@end direntry\n"
         "@end ifinfo\n"
         
         "@ifnottex\n"
         "@node Top, Modularization, (dir), (dir)\n"
         "@top Cfengine-Modularization\n"
         "@end ifnottex\n"
         
         "@ifhtml\n"
         "@html\n"
         "<a href=\"#Contents\"><h1>COMPLETE TABLE OF CONTENTS</h1></a>\n"
         
         "<h2>Summary of contents</h2>\n"
         
         "@end html\n"
         "@end ifhtml\n",
         VERSION,VERSION
         );

}

/*****************************************************************************/

void TexinfoFooter(FILE *fout)

{
 fprintf(fout,
         "@c =========================================================================\n"
         "@c @node Index,  , Cfengine Methods, Top\n"
         "@c @unnumbered Concept Index\n"
         "@c @printindex cp\n"
         "@c =========================================================================\n"
         
         "@ifhtml\n"
         "@html\n"
         "<a name=\"Contents\">\n"
         "@end html\n"
         "@end ifhtml\n"
         
         "@contents\n"
         "@c  The file is structured like a programming language. Each chapter\n"
         "@c  starts with a chapter comment.\n"
         "@c\n"
         "@c  Menus list the subsections so that an online info-reader can parse\n"
         "@c  the file hierarchically.\n"

         "@ifhtml\n"
         "@html\n"
         "<script type=\"text/javascript\">\n"
         "var gaJsHost = ((\"https:\" == document.location.protocol) ? \"https://ssl.\" : \"http://www.\");\n"
         "document.write(unescape(\"%%3Cscript src='\" + gaJsHost + \"google-analytics.com/ga.js' type='text/javascript'%%3E%%3C/script%%3E\"));\n"
         "</script>\n"
         "<script type=\"text/javascript\">\n"
         "var pageTracker = _gat._getTracker(\"UA-2576171-2\");\n"
         "pageTracker._initData();\n"
         "pageTracker._trackPageview();\n"
         "</script>\n"
         "@end html\n"
         "@end ifhtml\n"
         "@bye\n"
         );
}

/*****************************************************************************/

void TexinfoPromiseTypesFor(FILE *fout,struct SubTypeSyntax *st)

{ int i,j;
  char filename[CF_MAXVARSIZE];

/* Each array element is SubtypeSyntax representing an agent-promise assoc */

for (j = 0; st[j].btype != NULL; j++)
   {
   Verbose(" - Dealing with promise type %s\n",st[j].subtype);

   if (strcmp("*",st[j].btype) == 0)
      {
      fprintf(fout,"\n\n@node %s in common promises\n@section @code{%s} promises\n\n",st[j].subtype,st[j].subtype);
      snprintf(filename,CF_BUFSIZE-1,"promise_common_intro.texinfo");
      IncludeManualFile(fout,filename);
      TexinfoBodyParts(fout,st[j].bs,st[j].subtype);
      snprintf(filename,CF_BUFSIZE-1,"promise_common_example.texinfo");
      IncludeManualFile(fout,filename);
      snprintf(filename,CF_BUFSIZE-1,"promise_common_notes.texinfo");
      IncludeManualFile(fout,filename);            
      }
   else
      {
      fprintf(fout,"\n\n@node %s in %s promises\n@section @code{%s} promises in @samp{%s}\n\n",st[j].subtype,st[j].btype,st[j].subtype,st[j].btype);
      snprintf(filename,CF_BUFSIZE-1,"promise_%s_intro.texinfo",st[j].subtype);
      IncludeManualFile(fout,filename);
      TexinfoBodyParts(fout,st[j].bs,st[j].subtype);
      snprintf(filename,CF_BUFSIZE-1,"promise_%s_example.texinfo",st[j].subtype);
      IncludeManualFile(fout,filename);
      snprintf(filename,CF_BUFSIZE-1,"promise_%s_notes.texinfo",st[j].subtype);
      IncludeManualFile(fout,filename);
      }
   }
}

/*****************************************************************************/
/* Level                                                                     */
/*****************************************************************************/

void TexinfoBodyParts(FILE *fout,struct BodySyntax *bs,char *context)

{ int i;
  char filename[CF_MAXVARSIZE];
  
if (bs == NULL)
   {
   return;
   }
 
for (i = 0; bs[i].lval != NULL; i++)
   {
   Verbose(" - -  Dealing with body type %s\n",bs[i].lval);
   
   if (bs[i].range == (void *)CF_BUNDLE)
      {
      fprintf(fout,"\n\n@node %s in %s\n@subsection @code{%s}\n\n@b{Type}: %s (Separate Bundle) \n",bs[i].lval,context,bs[i].lval,CF_DATATYPES[bs[i].dtype]);
      }
   else if (bs[i].dtype == cf_body)
      {
      fprintf(fout,"\n\n@node %s in %s\n@subsection @code{%s} (compound body)\n@noindent @b{Type}: %s\n\n",bs[i].lval,context,bs[i].lval,CF_DATATYPES[bs[i].dtype]);
      TexinfoSubBodyParts(fout,(struct BodySyntax *)bs[i].range);
      }
   else
      {
      fprintf(fout,"\n\n@node %s in %s\n@subsection @code{%s}\n@noindent @b{Type}: %s\n\n",bs[i].lval,context,bs[i].lval,CF_DATATYPES[bs[i].dtype]);
      TexinfoShowRange(fout,(char *)bs[i].range,bs[i].dtype);
      fprintf(fout,"\n@noindent @b{Synopsis}: %s\n\n",bs[i].description);
      fprintf(fout,"\n@noindent @b{Example}:@*\n");
      snprintf(filename,CF_BUFSIZE-1,"bodypart_%s_example.texinfo",bs[i].lval);
      IncludeManualFile(fout,filename);
      fprintf(fout,"\n@noindent @b{Notes}:@*\n");
      snprintf(filename,CF_BUFSIZE-1,"bodypart_%s_notes.texinfo",bs[i].lval);
      IncludeManualFile(fout,filename);
      }
   }
}

/*******************************************************************/
/* Level                                                           */
/*******************************************************************/

void TexinfoShowRange(FILE *fout,char *s,enum cfdatatype type)

{ char *sp;
  struct Rlist *list = NULL,*rp;
 
if (strlen(s) == 0)
   {
   fprintf(fout,"@noindent @b{Allowed input range}: (arbitrary string)\n\n");
   return;
   }


if (type == cf_opts || type == cf_olist)
   {
   list = SplitStringAsRList(s,',');
   fprintf(fout,"@noindent @b{Allowed input range}: @*\n@example",s);
   
   for (rp = list; rp != NULL; rp=rp->next)
      {
      fprintf(fout,"\n          @code{%s}",rp->item);
      }

   fprintf(fout,"\n@end example\n");
   DeleteRlist(list);
   }
else
   {
   fprintf(fout,"@noindent @b{Allowed input range}: @code{%s}\n\n",s);
   }
}

/*****************************************************************************/

void TexinfoSubBodyParts(FILE *fout,struct BodySyntax *bs)

{ int i;
  char filename[CF_MAXVARSIZE];
  
if (bs == NULL)
   {
   return;
   }

fprintf(fout,"@table @samp\n");

for (i = 0; bs[i].lval != NULL; i++)
   {
   if (bs[i].range == (void *)CF_BUNDLE)
      {
      fprintf(fout,"@item %s\n@b{Type}: %s\n (Separate Bundle) \n\n",bs[i].lval,CF_DATATYPES[bs[i].dtype]);
      }
   else if (bs[i].dtype == cf_body)
      {
      fprintf(fout,"@item %s\n@b{Type}: %s\n\n",bs[i].lval,CF_DATATYPES[bs[i].dtype]);
      TexinfoSubBodyParts(fout,(struct BodySyntax *)bs[i].range);
      }
   else
      {
      fprintf(fout,"@item %s\n@b{Type}: %s\n\n",bs[i].lval,CF_DATATYPES[bs[i].dtype]);
      TexinfoShowRange(fout,(char *)bs[i].range,bs[i].dtype);
      fprintf(fout,"\n@noindent @b{Synopsis}: %s\n\n",bs[i].description);
      fprintf(fout,"\n@b{Example}:@*\n");
      snprintf(filename,CF_BUFSIZE-1,"bodypart_%s_example.texinfo",bs[i].lval);
      IncludeManualFile(fout,filename);
      fprintf(fout,"\n@b{Notes}:@*\n");
      snprintf(filename,CF_BUFSIZE-1,"bodypart_%s_notes.texinfo",bs[i].lval);
      IncludeManualFile(fout,filename);
      }
   }

fprintf(fout,"@end table\n");
}

/*****************************************************************************/

void IncludeManualFile(FILE *fout,char *file)

{ struct stat sb;
 char buffer[CF_BUFSIZE],filename[CF_BUFSIZE];
  FILE *fp;

AddSlash(MANDIR);
snprintf(filename,CF_BUFSIZE-1,"%s%s",MANDIR,file);

fprintf(fout,"@*\n");

if (stat(filename,&sb) == -1)
   {
   if ((fp = fopen(filename,"w")) == NULL)
      {
      CfOut(cf_inform,"fopen","Could not write to manual source %s",filename);
      return;
      }

   fprintf(fp,"\nFill me in (%s)\n",filename);
   fclose(fp);
   Verbose("Created %s template\n",filename);  
   }

if ((fp = fopen(filename,"r")) == NULL)
   {
   CfOut(cf_inform,"fopen","Could not read manual source %s\n",filename);
   fclose(fp);
   return;
   }

while(!feof(fp))
   {
   buffer[0] = '\0';
   fgets(buffer,CF_BUFSIZE,fp);
   fprintf(fout,"%s",buffer);
   }

fclose(fp);

fprintf(fout,"\n");
}
