/*
 *t由S打乱排序，再插入任意一个字符产生。给出t,s求插入的字符
*/
//利用异或运算
var findTheDifference = function( s,  t) {
    var c = 0;
    for (var i = 0; i < s.length; ++i) {
        c ^= s.charCodeAt(i);
    }
    for (var i = 0; i < t.length; ++i) {
        c ^= t.charCodeAt(i);
    }
    return String.fromCharCode(c);
}

//map对象
var findTheDifference = function(s, t) {
    var temp;
    var map = new Map();
    for(let i=0; i<s.length; i++){
        if(map.has(s[i])){
        	temp = map.get(s[i]);
        	temp++;
            map.set(s[i],temp);
        }else{
            map.set(s[i], 1);
        }
    }
    console.log(map);
    for(let i=0; i<t.length; i++){
        if(map.has(t[i])){
            temp = map.get(t[i]);
            temp--;
            map.set(t[i],temp);
        }else{
            return t[i];
        }
    }
    console.log(map);
    for(let key of map){
        if(key[1]==-1){
            return key[0];
        }
    }
};

var s = "abcda";
var t = "cdabaa";
console.log(findTheDifference(s, t));