/*无符号取反后的数
 *e.g.
 *input：5 101
 *output：2 010
*/
var findComplement = function(num) {
    var temp = num.toString(2);
    var result =[];
    console.log(temp);
    for(let i=0; i<temp.length; i++){
    	console.log(temp[i]==="0");
        temp[i]==="0"?result.push("1"):result.push("0");
    }
    return parseInt(result.join(""), 2);
};
var num = 0
var result = findComplement(num);
console.log(result);