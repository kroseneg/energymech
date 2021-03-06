/*

    EnergyMech, IRC bot software
    Copyright (c) 1997-2009 proton

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

*/
#define GENCMD_C
#include "config.h"
#include "structs.h"

/*

	These are defined in config.h

	DCC	0x00100		requires DCC
	CC	0x00200		requires commandchar
	PASS	0x00400		requires password / authentication
	CARGS	0x00800		requires args
	NOPUB	0x01000		ignore in channel (for password commands)
	NOCMD	0x02000		not allowed to be executed thru CMD
	GAXS	0x04000		check global access
	CAXS	0x08000		check channel access
	REDIR	0x10000		may be redirected
	LBUF	0x20000		can be linebuffered to server
	CBANG	0x40000		command may be prefixed with a bang (!)
	ACCHAN  0x80000		needs an active channel

	CLEVEL	0x000ff

*/

#define CCPW	CC|PASS

struct
{
	int	pass;
	char	*name;
	char	*func;
	ulong	flags;
	char	*cmdarg;

} pre_mcmd[] =
{
	/*
	 *  public access commands
	 */
	{ 0, "VERIFY",		"do_auth",		 0		| NOPUB				},
#ifdef TOYBOX
	{ 0, "8BALL",		"do_8ball",		 0		| CBANG				},
#endif /* TOYBOX */

	/*
	 *  Level 10
	 */
	{ 0, "ACCESS",		"do_access",		10	| CCPW					},
	{ 0, "BYE",		"do_bye",		10	| CC					},
	{ 0, "CHAT",		"do_chat",		10	| CCPW	| NOCMD				},
#ifdef RAWDNS
	{ 0, "DNS",		"do_dns",		10	| CCPW	| GAXS | CARGS			},
#endif /* RAWDNS */
	{ 0, "DOWN",		"do_opdeopme",		10	| CC	| CAXS				},
	{ 0, "ECHO",		"do_echo",		10	| CCPW	| CARGS				},
	{ 0, "HELP",		"do_help",		10	| CCPW	| REDIR | LBUF			},
	{ 0, "PASSWD",		"do_passwd",		10	| PASS	| NOPUB | CARGS			},
#ifdef DCC_FILE
	{ 0, "SEND",		"do_send",		10	| CC	| NOCMD	| CBANG	| CARGS		},
#endif /* DCC_FILE */
	{ 0, "USAGE",		"do_usage",		10	| CCPW	| REDIR	| CARGS			},

	/*
	 *  Level 20
	 */
	{ 0, "ONTIME",		"do_upontime",		20	| CCPW					, "Ontime: %s" },
	{ 0, "UPTIME",		"do_upontime",		20	| CCPW					, "Uptime: %s" },
	{ 0, "VER",		"do_version",		20	| CCPW					},
	{ 0, "WHOM",		"do_whom",		20	| CCPW	| REDIR | LBUF			},
#ifdef SEEN
	{ 0, "SEEN",		"do_seen",		20	| CCPW	| CBANG				},
#endif /* SEEN */

	/*
	 *  Level 40
	 */
	{ 0, "BAN",		"do_kickban",		40	| CCPW	| CAXS | CARGS | ACCHAN		, "\\x00ban\\0bann" },
	{ 0, "BANLIST",		"do_banlist",		40	| CCPW	| CAXS | DCC | REDIR | LBUF | ACCHAN },
	{ 0, "CCHAN",		"do_cchan",		40	| CCPW					},	/* global_access ? */
	{ 0, "CSERV",		"do_cserv",		40	| CCPW					},
	{ 0, "CHANNELS",	"do_channels",		40	| CCPW	| DCC				},
	{ 0, "DEOP",		"do_opvoice",		40	| CCPW	| CAXS | CARGS			, "o-" },
	{ 0, "ESAY",		"do_esay",		40	| CCPW	| CAXS | CARGS			},
	{ 0, "IDLE",		"do_idle",		40	| CCPW	| CARGS				},
	{ 0, "INVITE",		"do_invite",		40	| CCPW	| CAXS | ACCHAN			},
	{ 0, "KB",		"do_kickban",		40	| CCPW	| CAXS | CARGS | ACCHAN		, "\\x04kickban\\0kickbann" },
	{ 0, "KICK",		"do_kickban",		40	| CCPW	| CAXS | CARGS | ACCHAN		, "\\x07kick\\0kick" },
	{ 0, "LUSERS",		"do_irclusers",		40	| CCPW	| DCC | REDIR | LBUF		},
	{ 0, "ME",		"do_sayme",		40	| CCPW	| CARGS				},
	{ 0, "MODE",		"do_mode",		40	| CCPW	| CARGS				},
	{ 0, "NAMES",		"do_names",		40	| CCPW					},
	{ 0, "OP",		"do_opvoice",		40	| CCPW	| CAXS				, "o+" },
	{ 0, "SAY",		"do_sayme",		40	| CCPW	| CARGS				},
	{ 0, "SCREW",		"do_kickban",		40	| CCPW	| CAXS | CARGS | ACCHAN		, "\\x02screwban\\0screwbann" },
	{ 0, "SET",		"do_set",		40	| CCPW					},
	{ 0, "SITEBAN",		"do_kickban",		40	| CCPW	| CAXS | CARGS | ACCHAN		, "\\x01siteban\\0sitebann" },
	{ 0, "SITEKB",		"do_kickban",		40	| CCPW	| CAXS | CARGS | ACCHAN		, "\\x05sitekickban\\0sitekickbann" },
	{ 0, "TIME",		"do_time",		40	| CCPW					},
	{ 0, "TOPIC",		"do_topic",		40	| CCPW	| CAXS | CARGS | ACCHAN		},
	{ 0, "UNBAN",		"do_unban",		40	| CCPW	| CAXS				},
	{ 0, "UNVOICE",		"do_opvoice",		40	| CCPW	| CAXS | CARGS			, "v-" },
	{ 0, "UP",		"do_opdeopme",		40	| CCPW	| CAXS				},
	{ 0, "USER",		"do_user",		40	| CCPW	| CARGS				},
	{ 0, "USERHOST",	"do_ircwhois",		40	| CCPW	| CARGS				},
	{ 0, "VOICE",		"do_opvoice",		40	| CCPW	| CAXS				, "v+" },
	{ 0, "WALL",		"do_wall",		40	| CCPW	| CAXS | CARGS | ACCHAN		},
	{ 0, "WHO",		"do_showusers",		40	| CCPW	| CAXS | DCC			},
	{ 0, "WHOIS",		"do_ircwhois",		40	| CCPW	| CARGS | DCC | REDIR | LBUF	},
#ifdef NOTE
	{ 0, "NOTE",		"do_note",		40	| CCPW	| CARGS				},
	{ 0, "READ",		"do_read",		40	| CCPW					},
#endif /* NOTE */
#ifdef STATS
	{ 0, "INFO",		"do_info",		40	| CCPW	| CAXS | DCC			},
#endif /* STATS */

	/*
	 *  Level 50
	 */
	{ 0, "QSHIT",		"do_shit",		50	| CCPW	| CARGS				},
	{ 0, "RSHIT",		"do_rshit",		50	| CCPW	| CARGS				},
	{ 0, "SHIT",		"do_shit",		50	| CCPW	| CARGS				},
	{ 0, "SHITLIST",	"do_shitlist",		50	| CCPW	| DCC | REDIR | LBUF		},
#ifdef GREET
	{ 0, "GREET",		"do_greet",		50	| CCPW	| CARGS				},
#endif /* GREET */
#ifdef TOYBOX
	{ 0, "INSULT",		"do_random_msg",	50	| CCPW					, RANDINSULTFILE },
	{ 0, "PICKUP",		"do_random_msg",	50	| CCPW					, RANDPICKUPFILE },
	{ 0, "RSAY",		"do_random_msg",	50	| CCPW					, RANDSAYFILE },
	{ 0, "RT",		"do_randtopic",		50	| CCPW	| CAXS | ACCHAN			},
#endif /* TOYBOX */
#ifdef TRIVIA
	{ 0, "TRIVIA",		"do_trivia",		50	| CCPW	| CAXS | CARGS | CBANG		},
#endif /* TRIVIA */

	/*
	 *  Level 60
	 */
	{ 0, "SHOWIDLE",	"do_showidle",		60	| CCPW	| CAXS | DCC | ACCHAN		},
	{ 0, "USERLIST",	"do_userlist",		60	| CCPW	| DCC				},
#ifdef CTCP
	{ 0, "CTCP",		"do_ping_ctcp",		60	| CCPW	| CARGS				},
	{ 0, "PING",		"do_ping_ctcp",		60	| CCPW	| CARGS				},
#endif /* CTCP */

	/*
	 *  Level 70 == JOINLEVEL
	 */
	{ 0, "CYCLE",		"do_cycle",		70	| CCPW	| CAXS | ACCHAN			},
	{ 0, "FORGET",		"do_forget",		70	| CCPW	| CARGS				},
	{ 0, "JOIN",		"do_join",		70	| CCPW	| CARGS				},
	{ 0, "KS",		"do_kicksay",		70	| CCPW	| REDIR | LBUF			},
	{ 0, "PART",		"do_part",		70	| CCPW	| CAXS | ACCHAN			},
	{ 0, "RKS",		"do_rkicksay",		70	| CCPW	| CARGS				},
	{ 0, "SETPASS",		"do_setpass",		70	| CCPW	| NOPUB	| CARGS			},
#ifdef NOTIFY
	{ 0, "NOTIFY",		"do_notify",		70	| CCPW	| DCC | GAXS | REDIR | LBUF	},
#endif /* NOTIFY */

	/*
	 *  Level 80 == ASSTLEVEL
	 */
	{ 0, "AWAY",		"do_away",		80	| CCPW	| GAXS				},
#if defined(BOTNET) && defined(REDIRECT)
	{ 0, "CMD",		"do_cmd",		80	| CCPW	| CARGS				},
#endif /* BOTNET && REDIRECT */
	{ 0, "LAST",		"do_last",		80	| CCPW	| DCC				},
	{ 0, "LOAD",		"do_load",		80	| CCPW	| GAXS				},
	{ 0, "MSG",		"do_msg",		80	| CCPW	| CARGS				},
	{ 0, "NEXTSERVER",	"do_server",		80	| CCPW	| GAXS				},
	{ 0, "SAVE",		"do_save",		80	| CCPW	| GAXS				},
	{ 0, "SERVER",		"do_server",		80	| CCPW	| GAXS | REDIR | LBUF		},
	{ 0, "SERVERGROUP",	"do_servergroup",	80	| CCPW	| GAXS | REDIR | LBUF		},
	{ 0, "STATS",		"do_ircstats",		80	| CCPW	| DCC | CARGS			},
#ifdef ALIAS
	{ 0, "ALIAS",		"do_alias",		80	| CCPW	| GAXS				},
	{ 0, "UNALIAS",		"do_unalias",		80	| CCPW	| GAXS | CARGS			},
#endif /* ALIAS */
#ifdef TOYBOX
	{ 0, "BIGSAY",		"do_bigsay",		80	| CCPW	| CAXS | CARGS			},
#endif /* TOYBOX */

	/*
	 *  Level 90
	 */
	{ 0, "CLEARSHIT",	"do_clearshit",		90	| CCPW	| GAXS				},
	{ 0, "DO",		"do_do",		90	| CCPW	| GAXS | CARGS			},
	{ 0, "NICK",		"do_nick",		90	| CCPW	| GAXS | CARGS			},
	{ 0, "RSPY",		"do_rspy",		90	| CCPW	| CARGS				},
	{ 0, "SPY",		"do_spy",		90	| CCPW					},
#ifdef BOTNET
	{ 0, "LINK",		"do_link",		90	| CCPW	| GAXS				},
#endif /* BOTNET */
#ifdef DYNCMD
	{ 0, "CHACCESS",	"do_chaccess",		90	| CCPW	| GAXS | CARGS			},
#endif /* DYNCMD */
#ifdef UPTIME
	{ 0, "UPSEND",		"do_upsend",		90	| CCPW	| GAXS				},
#endif /* UPTIME */

	/*
	 *  Level 100
	 */
#ifdef RAWDNS
	{ 0, "DNSSERVER",	"do_dnsserver",		100	| CCPW  | GAXS				},
	{ 0, "DNSROOT",		"do_dnsroot",		100	| CCPW	| GAXS | CARGS			},
#endif /* RAWDNS */
	{ 0, "CORE",		"do_core",		100	| CCPW	| DCC				},
	{ 0, "DIE",		"do_die",		100	| CCPW	| GAXS				},
	{ 0, "RESET",		"do_reset",		100	| CCPW	| GAXS | NOCMD			},
	{ 0, "SHUTDOWN",	"do_shutdown",		100	| CCPW	| GAXS | NOPUB | NOCMD		},
#ifdef DEBUG
	{ 0, "DEBUG",		"do_debug",		100	| CCPW	| GAXS				},
#endif /* DEBUG */
#ifdef PYTHON
#ifdef PLEASE_HACK_MY_SHELL 
	{ 0, "PYTHON",		"do_python",		100	| CCPW	| GAXS | CARGS			},
#endif /* PLEASE_HACK_MY_SHELL */
	{ 0, "PYTHONSCRIPT",	"do_pythonscript",	100	| CCPW	| GAXS | CARGS			},
#endif /* PYTHON */
#ifdef TCL
#ifdef PLEASE_HACK_MY_SHELL
	{ 0, "TCL",		"do_tcl",		100	| CCPW	| GAXS | CARGS			},
#endif /* PLEASE_HACK_MY_SHELL */
	{ 0, "TCLSCRIPT",	"do_tcl",		100	| CCPW	| GAXS | CARGS			},
#endif /* TCL */
	/*---*/
	{ 0, NULL,		NULL,			0						},
};

#define __define_strng	4
#define __define_print	3
#define __struct_acces	2
#define __struct_print	1

int main(int argc, char **argv)
{
	FILE	*of;
	usercombo combo;
	char	tmp[100];
	char	*pt,*tabs;
	int	i,j,wh;
	int	pass;
	int	ct;
	int	sl;
	OnMsg	v;

	pass = __define_strng;
	ct = 0;

	printf("/""* This file is automatically generated from gencmd.c *""/\n");
	printf("#ifndef MCMD_H\n#define MCMD_H 1\n\n");

	while(pass)
	{
		if (pass == __struct_print)
		{
			printf("LS const OnMsg mcmd[] =\n{\n");
		}
		if (pass == __struct_acces)
		{
			printf("LS OnMsg_access acmd[] = \n{\n");
		}
		for(i=0;pre_mcmd[i].name;i++)
		{
			pt = 0;
			wh = 0;
			for(j=0;pre_mcmd[j].name;j++)
			{
				if (pre_mcmd[j].pass != pass)
				{
					pt = pre_mcmd[j].name;
					wh = j;
					break;
				}
			}
			for(j=0;pre_mcmd[j].name;j++)
			{
				if ((pre_mcmd[j].pass != pass) && (strcmp(pt,pre_mcmd[j].name) > 0))
				{
					pt = pre_mcmd[j].name;
					wh = j;
				}
			}
			if (pass == __define_strng)
			{
				//printf("#define S_%s%s\t\"%s\"\n",pt,((strlen(pt) + 2) < 8) ? "\t" : "",pt);
			}
			if (pass == __define_print)
			{
				//printf("#define C_%s%s\tmcmd[%i].name\n",pt,((strlen(pt) + 2) < 8) ? "\t" : "",ct);
				printf("BEG const char C_%s[]%s\tMDEF(\"%s\");\n",pt,((strlen(pt) + 3) < 8) ? "\t" : "",pt);
				ct++;
			}
			if (pass == __struct_acces)
			{
				printf("\t%i,\t/""* %s *""/\n",
					pre_mcmd[wh].flags & CLEVEL,
					pt);
			}
			if (pass == __struct_print)
			{
				memset(&v,0,sizeof(v));

				v.defaultaccess = pre_mcmd[wh].flags & CLEVEL;
				/* + defaultaccess */
				v.dcc    = (pre_mcmd[wh].flags & DCC)    ? 1 : 0;
				v.cc     = (pre_mcmd[wh].flags & CC)     ? 1 : 0;
				v.pass   = (pre_mcmd[wh].flags & PASS)   ? 1 : 0;
				v.args   = (pre_mcmd[wh].flags & CARGS)  ? 1 : 0;
				v.nopub  = (pre_mcmd[wh].flags & NOPUB)  ? 1 : 0;
				v.nocmd  = (pre_mcmd[wh].flags & NOCMD)  ? 1 : 0;
				v.gaxs   = (pre_mcmd[wh].flags & GAXS)   ? 1 : 0;
				v.caxs   = (pre_mcmd[wh].flags & CAXS)   ? 1 : 0;
				v.redir  = (pre_mcmd[wh].flags & REDIR)  ? 1 : 0;
				v.lbuf   = (pre_mcmd[wh].flags & LBUF)   ? 1 : 0;
				v.cbang  = (pre_mcmd[wh].flags & CBANG)  ? 1 : 0;
				v.acchan = (pre_mcmd[wh].flags & ACCHAN) ? 1 : 0;

				sprintf(tmp,"%3i,%2i,%2i,%2i,%2i,%2i,%2i,%2i,%2i,%2i,%2i,%2i,%2i",
					v.defaultaccess,
					v.dcc,
					v.cc,
					v.pass,
					v.args,
					v.nopub,
					v.nocmd,
					v.gaxs,
					v.caxs,
					v.redir,
					v.lbuf,
					v.cbang,
					v.acchan
					);

				sl = strlen(pre_mcmd[wh].func) + 1;
				tabs = "\t\t\t";

				sl = (sl & ~7) / 8;
				tabs += sl;

				printf(	(pre_mcmd[wh].cmdarg) ? "{ C_%s,%s\t%s,%s%s\t, \"%s\"\t},\n" : "{ C_%s,%s\t%s,%s%s\t},\n",
					pt,
					((strlen(pt) + 5) < 8) ? "\t" : "",
					pre_mcmd[wh].func,
					tabs,
					tmp,
					pre_mcmd[wh].cmdarg
				);
			}
			pre_mcmd[wh].pass = pass;
		}
		if (pass == __define_strng)
		{
			/* nothing */
		}
		if (pass == __define_print)
		{
			printf("\n#ifdef MAIN_C\n\n");
		}
		if (pass == __struct_print)
		{
			printf("{ NULL, }};\n\n");
		}
		if (pass == __struct_acces)
		{
			printf("};\n\n");
		}
		pass--;
	}
	printf("#define LOCALHOST_ULONG %lu\n",inet_addr("127.1"));
	printf("#else /""* MAIN_C *""/\n\n");
	printf("extern OnMsg mcmd[];\n");
	printf("extern OnMsg_access acmd[];\n\n");
	printf("#endif /""* MAIN_C *""/\n\n");
	printf("#endif /""* MCMD_H *""/\n\n");

	unlink("usercombo.h");
	of = fopen("usercombo.h","w");

	fprintf(of,"/""* This file is automatically generated from gencmd.c *""/\n");

#ifdef BOTNET
	combo.comboflags = 0; combo.x.noshare = 1;
	fprintf(of,"#define COMBO_NOSHARE\t0x%x\n",combo.comboflags);
	combo.comboflags = 0; combo.x.readonly = 1;
	fprintf(of,"#define COMBO_READONLY\t0x%x\n",combo.comboflags);
#endif /* BOTNET */

#ifdef GREET
	combo.comboflags = 0; combo.x.greetfile = 1;
	fprintf(of,"#define COMBO_GREETFILE\t0x%x\n",combo.comboflags);
	combo.comboflags = 0; combo.x.randline = 1;
	fprintf(of,"#define COMBO_RANDLINE\t0x%x\n",combo.comboflags);
#endif /* GREET */

#ifdef BOUNCE
	combo.comboflags = 0; combo.x.bounce = 1;
	fprintf(of,"#define COMBO_BOUNCE\t0x%x\n",combo.comboflags);
#endif /* BOUNCE */

	combo.comboflags = 0; combo.x.echo = 1;
	fprintf(of,"#define COMBO_ECHO\t0x%x\n",combo.comboflags);
	combo.comboflags = 0; combo.x.aop = 1;
	fprintf(of,"#define COMBO_AOP\t0x%x\n",combo.comboflags);
	combo.comboflags = 0; combo.x.avoice = 1;
	fprintf(of,"#define COMBO_AVOICE\t0x%x\n",combo.comboflags);

	fclose(of);
	return(0);
}
