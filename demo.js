var nlpir = require("./build/Release/nlpir");
var s = "反省中：在餐馆吃饭，等米饭太久，失去耐心，对服务员大吼，瞬间变成我鄙视的人";
nlpir.init('./NLPIR');
nlpir.importDictFile("./NLPIR/userdict.txt");
nlpir.segment(s, function(msg){
	console.log(msg);
});
