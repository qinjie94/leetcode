/*
 *A=1, ……Z =26,AA=27, 给出s="A..."求数字
 */var titleToNumber = function(s) {
    let values = "0ABCDEFGHIJKLMNOPQRSTUVWXYZ";//[0,"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z"];
    let temp = s.split("");
    let result = 0;
    for(let i=0; i<temp.length; i++){
    	result = result*26+values.indexOf(temp[i])
    }
    return result;
};
console.log(titleToNumber("Z"));