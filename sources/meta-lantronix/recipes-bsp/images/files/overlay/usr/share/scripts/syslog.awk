#! /bin/awk -f
BEGIN {
    Severity["eme"] = 0
    Severity["ale"] = 1
    Severity["cri"] = 2
    Severity["err"] = 3
    Severity["war"] = 4
    Severity["not"] = 5
    Severity["inf"] = 6
    Severity["deb"] = 7
    Severity["non"] = 8
    log_level = tolower(substr(log_level,1,3))
    if (log_level in Severity) {}
    else { exit}
    log_level = Severity[log_level]
    Month["Jan"] = 101
    Month["Feb"] = 102
    Month["Mar"] = 103
    Month["Apr"] = 104
    Month["May"] = 105
    Month["Jun"] = 106
    Month["Jul"] = 107
    Month["Aug"] = 108
    Month["Sep"] = 109
    Month["Oct"] = 110
    Month["Nov"] = 111
    Month["Dec"] = 112
    "date +%Y" | getline current_year
    offset_current_year = (current_year-src_year)*12
    offset_based_years = (dst_year-src_year)*12
    dst_day += 10
    src_day += 10
    dst_mnth = Month[dst_month]+offset_based_years+100
    src_mnth = Month[src_month]+100
}
(dst_mnth dst_day dst_time < Month[$1]+offset+100+offset_current_year $2+10 $3) && matched {
    exit
}
Month[$1]+offset+100+offset_current_year $2+10 $3 >= src_mnth src_day src_time, dst_mnth dst_day dst_time < Month[$1]+offset+100+offset_current_year $2+10 $3 {
    if(matched==0)
        matched=1
    prev_month=curr_month
    curr_month=$1
    if(prev_month && Month[prev_month] > Month[curr_month])
        offset += 12
    split($5, log_details, ".")
    if (2 in log_details && ((current_level = tolower(substr(log_details[2],1,3))) in Severity)) { current_level = Severity[current_level]}
    else current_level = 0
    if(current_level <= log_level)
        print
    else{}
}
