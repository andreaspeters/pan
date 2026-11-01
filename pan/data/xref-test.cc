#include <config.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <pan/general/string-view.h>
#include <pan/general/test.h>
#include "xref.h"
#include "article.h"

using namespace pan;

int
main (void)
{
   const Quark s1 ("server1");
   const Quark s2 ("server2");
   const Quark s3 ("server3");
   const Quark s4 ("server4");
   const Quark g1 ("group1");
   const Quark g2 ("group2");
   const Quark g3 ("group3");

   /**
   ***  Add
   **/

   Xref s;
   s.insert (s1, g1, Article_Number (100ul));
   check (s.size() == 1)
   s.insert (s2, g1, Article_Number (200ul));
   check (s.size() == 2)
   s.insert (s3, g1, Article_Number (300ul));
   check (s.size() == 3)
   s.insert (s1, g1, Article_Number (300ul)); // should fail -- already have server+group
   check (s.size() == 3)
   s.insert (s1, g2, Article_Number (300ul));
   check (s.size() == 4)

   /**
   ***  Find Number
   **/

#if 0
   uint64_t number;
   check (s.find (s1, g1, number))
   check (number == 100)
   check (s.find (s2, g1, number))
   check (number == 200)
   check (s.find (s3, g1, number))
   check (number == 300)
   check (s.find (s1, g2, number))
   check (number == 300)
   check (!s.find (s1, g3, number))
   check (!s.find (s2, g3, number))
   check (!s.find (s4, g1, number))
#endif

   /**
   *** Remove Server
   **/

   s.remove_server (s1);
   check (s.size() == 2)
   for (Xref::targets_t::const_iterator it=s.begin(), end=s.end(); it!=end; ++it)
      check (it->server != s1)

   /**
   *** Clear
   **/

   s.clear ();
   check (s.empty())

   /**
   *** Xref: header
   **/

   StringView v ("news.netfront.net alt.zen:175094 alt.religion.buddhism:10033 talk.religion.buddhism:170169");
   s.insert ("netfront", v);
   check (s.size() == 3)
   v = "news.froody.org alt.zen:666 alt.religion.buddhism:999 talk.religion.buddhism:333";
   s.insert ("froody", v);
   check (s.size() == 6)

  quarks_t servers;
  s.get_servers (servers);
  check (servers.size()==2)
  check (servers.count("froody"))
  check (servers.count("netfront"))

  check (s.has_server ("froody"))
  check (s.has_server ("netfront"))
  check (!s.has_server ("foo"))

//for (Xref::const_iterator it(s.begin()), end(s.end()); it!=end; ++it) std::cerr << "server [" << it->server << "], group [" << it->group << "], number [" << it->number << "]" << std::endl;
  s.remove_server ("froody");
  check (s.size() == 3);
  check (!s.has_server ("froody"));
  check (s.has_server ("netfront"));

  // Synchronet historically emitted the Xref field without whitespace
  // after its colon ("Xref:host group:number"). It must not be parsed as
  // a newsgroup named "Xref".
  Xref synchronet_xref;
  synchronet_xref.insert (Quark ("synchronet"), StringView ("xReF:bbs.example Local.General:24"));
  Quark synchronet_group;
  Article_Number synchronet_number (0ul);
  check (synchronet_xref.find ("synchronet", synchronet_group, synchronet_number));
  check (synchronet_group == "Local.General");
  check (synchronet_number == Article_Number (24ul));

  Xref conventional_xref;
  conventional_xref.insert (Quark ("other"), StringView ("Xref: bbs.example alt.test:5"));
  check (conventional_xref.find ("other", synchronet_group, synchronet_number));
  check (synchronet_group == "alt.test");
  check (synchronet_number == Article_Number (5ul));

  s.remove_targets_less_than ("netfront", "alt.zen", Article_Number (100000ul));
  check (s.size() == 3);
  s.remove_targets_less_than ("netfront", "alt.zen", Article_Number (200000ul));
  check (s.size() == 2);

  Quark group;
  Article_Number number (0ul);
  check (s.find ("netfront", group, number))
  check (number != Article_Number (175094ul))
  check (number == Article_Number (10033ul) || number == Article_Number (107169ul))
  check (number == Article_Number (10033ul) ? group == "alt.religion.buddhism" : group == "talk.religion.buddhism")

  Xref x;
  x.insert (s);
  x.insert (s);
  check (x.size() == 2)

  return 0;
}
