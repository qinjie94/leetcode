/*
 *找出字符串中不重复的最长子串的长度
*/
var lengthOfLongestSubstring = function(s) {
    var map = new Map();
    var record = 0;
    var result = "";
    for(let i=0; i<s.length; i++){
        if(map.has(s[i])){
            record = Math.max(record, result.length);
            i = map.get(s[i])
            map.clear();
            result = "";
        }else{
            map.set(s[i], i);
            result = result+s[i]
        }
    }
    record = Math.max(record, result.length);
    return record;
};
var a = "dvdf";
console.log(a.slice());
console.log(lengthOfLongestSubstring(a));