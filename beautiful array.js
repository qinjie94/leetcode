/*我们假设n= 6，k = 5，那么这个序列就是 6 1 5 2 4 3 
形成的k个元素差为： 5 4 3 2 1 （反向也是可以的 即 1 6 2 5 3 4）
若k不等于n-1，我们只需要按上述规律形成满足k-1的序列，
剩余序列按递减序即可（剩余的差值都为1）。 
*/
var constructArray = function(n, k) {
    let result = [];
    let start = k+1;
    let i = 1;
    let j = k+1;
    while(i<=j){
    		result.push((k--)%2==1?i++:j--)
    }
    for(let g=start+1; g<=n; g++){
    	console.log(g);
    	result.push(g);
    }
    return result;
};
console.log(constructArray(5,1));