#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>
#include "../../common/utils/json_utils.h"
#include "../../common/utils/utils.h"
#include "../../common/spi_utils/spi_utils.h"

using namespace std;

int parsefile(const char *idstr)
{
    char filename[256];
    sprintf(filename, "html/%s", idstr);
    char *buf = readfile(filename);
    if (!buf) {
        return 1;
    }
    string bstr = string(buf);
    free(buf);
    map <string, string> kv;
    kv["site_id"] = "haoshouyi";
    kv["project_name"] = extract(bstr, "<meta name=","<meta http-equiv=", "<title>"," _投资项目");
  //  kv["borrower"] = extract(bstr,"<div class=\"top-l\">","<span class=","title=\"","");
    kv["project_id"] = extract(bstr,"<p class=\"y_p\">","<li class=\"y_li\">","项目编号：","</span></li>");
    kv["borrowing_amount"] = extract(bstr,"<li class=\"y_li1\">","借款:","<span>","</span>");
    string::size_type pos = kv["borrowing_amount"].find("万");
    if (pos != string::npos){
        kv["borrowing_amount"] = doubletostring(atof(filternum(kv["borrowing_amount"]).c_str())*10000);
    } else{
        kv["borrowing_amount"] = doubletostring(atof(filternum(kv["borrowing_amount"]).c_str()));
    }  

    kv["annulized_rating"] = filternum(extract(bstr,"<li class=\"y_li1\">","年化利率:", "<span>", "%</span></p>"));
    kv["payment_method"] = extract(bstr,"起息日","valign=\"middle\">","valign=\"middle\">","</td>");
    kv["loan_period"] = extract(bstr, "<li class=\"y_li1\">", "期限:", "<span>","</span></p><p>");
    int pos1 = kv["loan_period"].find("月");
    if (pos1 != -1) {
        kv["loan_period"] = longtostring(atof(kv["loan_period"].c_str())*30);
    }
    else{

        kv["loan_period"] = longtostring(atof(kv["loan_period"].c_str()));
    }

    // kv["invested_amount"] = doubletostring(s_atod(kv["borrowing_amount"].c_str()) - s_atod(filternum(extract(bstr,"剩余时间","额","￥","")).c_str()));
     long long etime1 = stringtotime(extract(bstr,"\"lxftime\"></div></td>","<td width=\"100","起息日:","</td>").c_str(), "%Y-%m-%d %H:%M:%S");
     if (etime1 > 1000000000) {
          kv["release_time"] = longtostring(etime1);
     }
 //      kv["end_time"] = extract(bstr,"<ul class=\"d_date\">","<s>","满标时间：","</li>");  
  //  kv["project_details"] = extract(bstr,"role=\"tablist\">","借款详情","id=\"jkxq\">","<div class=\"bid_jkzl\">");
    {
            string tmp;
            string::size_type startpos = bstr.find("状态");
            if (startpos != string::npos) {
                string::size_type endpos = bstr.find("</table>", startpos);
                if (endpos != string::npos) {
                    tmp = bstr.substr(startpos, endpos - startpos);
                }
            }
            string ivname;
            string ivaccount;
            string ivtime;
            string retstr;
            while ((ivname = extract(tmp, "<td width=", "middle\">", "middle\">", "</td>")) != "") {
                ivaccount = extract(tmp, "middle\">", "%", "￥","</td>");
                ivtime = extract(tmp, "%", "￥", "middle\">", "</td>");
                ivtime = longtostring(stringtotime(ivtime.c_str(), "%Y-%m-%d %H:%M:%S"));
                retstr += ivtime + "|" + ivname + "|" + ivaccount + "|";
                string::size_type pos = tmp.find("align=\"top\" /></td>");
                if (pos != string::npos) {
                    tmp = tmp.substr(pos + 1);
                }
                else {
                   break;
            }
        }
      kv["investor"] = retstr;    
    }


    if (kv["project_id"] == "" ) {
        return 1;
    }
   printstringmap(kv);
    sprintf(filename, "data/%s", idstr);
    return savestringmap(kv, filename);
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        return 0;
    }
    return parsefile(argv[1]);
}
