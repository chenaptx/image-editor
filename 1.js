var gauss_size=document.getElementById("gauss_size");
var gauss_sigma=document.getElementById("gauss_sigma");
var gauss_anchor_x=document.getElementById("gauss_anchor_x");
var gauss_anchor_y=document.getElementById("gauss_anchor_y");
var gauss_submit=document.getElementById("gauss_submit");

gauss_submit.onclick=function(event){
    
    ipc.send("gauss",gauss_size.value,gauss_sigma.value,gauss_anchor_x.value,gauss_anchor_y.value);
}

var median_size=document.getElementById("median_size");
var median_anchor_x=document.getElementById("median_anchor_x");
var median_anchor_y=document.getElementById("median_anchor_y");
var median_submit=document.getElementById("median_submit");

median_submit.onclick=function(event){
    ipc.send("median",median_size.value,median_anchor_x.value,median_anchor_y.value);
}
var mean_size=document.getElementById("mean_size");
var mean_anchor_x=document.getElementById("mean_anchor_x");
var mean_anchor_y=document.getElementById("mean_anchor_y");
var mean_submit=document.getElementById("mean_submit");

mean_submit.onclick=function(event){
    ipc.send("mean",mean_size.value,mean_anchor_x.value,mean_anchor_y.value);
}

var add=document.getElementById("add");
add.addEventListener("click",function(e){
    ipc.send("add");
})
var substract=document.getElementById("substract");
substractadd.addEventListener("click",function(e){
    ipc.send("substract");
})
var mutiply=document.getElementById("mutiply");
mutiply.addEventListener("click",function(e){
    ipc.send("mutiply");
})
var divide=document.getElementById("divide");
divide.addEventListener("click",function(e){
    ipc.send("divide");
})

var scale=document.getElementById("scale");
scale.oninput=function(e){
    ipc.send("scale",scale.value);

}
var rotate=document.getElementById("rotate");
rotate.oninput=function(e){
    ipc.send("rotate",rotate.value);

}
var cut=document.getElementById("cut");
cut.addEventListener("click",function(e){
    var cut_x=document.getElementById("cut_x").value;
    var cut_y=document.getElementById("cut_y").value;
    var cut_w=document.getElementById("cut_w").value;
    var cut_h=document.getElementById("cut_h").value;
    ipc.send("cut",cut_x,cut_y,cut_w,cut_h);
})

var distancetransform=document.getElementById("distancetransform");
distancetransform.addEventListener("click",function(e){
    ipc.send("distancetransform");
})
var skeleton=document.getElementById("skeleton");
skeleton.addEventListener("click",function(e){
    ipc.send("skeleton");
})
var skeletonrebuild=document.getElementById("skeletonrebuild");
skeletonrebuild.addEventListener("click",function(e){
    ipc.send("skeletonrebuild");
})
var elem=document.getElementById("elem");
var dilate=document.getElementById("dilate");
dilate.addEventListener("click",function(e){
    var v=elem.value;
    console.log(v);
    ipc.send("dilate",v);
})
var erode=document.getElementById("erode");
erode.addEventListener("click",function(e){
    var v=elem.value;
    console.log(v);
    ipc.send("erode",v);
})
var openning=document.getElementById("openning");
openning.addEventListener("click",function(e){
    var v=elem.value;
    console.log(v);
    ipc.send("openning",v);
})
var closing=document.getElementById("closing");
closing.addEventListener("click",function(e){
    var v=elem.value;
    console.log(v);
    ipc.send("closing",v);
})
var Mrebuild=document.getElementById("Mrebuild");
Mrebuild.addEventListener("click",function(e){
    var v=elem.value;
    console.log(v);
    ipc.send("Mrebuild",v);
})
var window=document.getElementById("window");
var thick=document.getElementById("thick");
thick.addEventListener("click",function(e){
    var v=elem.value;
    var vv=window.value;
    console.log(vv);
    ipc.send("thick",v,vv);
})
var thin=document.getElementById("thin");
thin.addEventListener("click",function(e){
    var v=elem.value;
    var vv=window.value;
    console.log(vv);
    ipc.send("thin",v,vv);
})

var Gdilate=document.getElementById("Gdilate");
Gdilate.addEventListener("click",function(e){
    var v=elem.value;
    console.log(v);
    ipc.send("Gdilate",v);
})
var Gerode=document.getElementById("Gerode");
Gerode.addEventListener("click",function(e){
    var v=elem.value;
    console.log(v);
    ipc.send("Gerode",v);
})
var Gopenning=document.getElementById("Gopenning");
Gopenning.addEventListener("click",function(e){
    var v=elem.value;
    console.log(v);
    ipc.send("Gopenning",v);
})
var Gclosing=document.getElementById("Gclosing");
Gclosing.addEventListener("click",function(e){
    var v=elem.value;
    console.log(v);
    ipc.send("Gclosing",v);
})
var GMrebuild=document.getElementById("GMrebuild");
GMrebuild.addEventListener("click",function(e){
    var v=elem.value;
    console.log(v);
    ipc.send("GMrebuild",v);
})


var Binary_ostu=document.getElementById("Binary_ostu");
Binary_ostu.addEventListener("onclick",function(e){
    ipc.send("Binary_ostu");
})
var high=document.getElementById("high");
high.oninput=function(e){
    var v=high.value;
    var vv=low.value;
    ipc.send("Binary",vv,v);

}
var low=document.getElementById("low");
low.oninput=function(e){
    var v=high.value;
    var vv=low.value;
    ipc.send("Binary",vv,v);

}
var detect_sobel=document.getElementById("detect_sobel");
detect_sobel.addEventListener("onclick",function(e){
    ipc.send("detect_sobel");
})
var detect_laplace=document.getElementById("detect_laplace");
detect_laplace.addEventListener("onclick",function(e){
    ipc.send("detect_laplace");
})
var detect_canny=document.getElementById("detect_canny");
detect_canny.addEventListener("onclick",function(e){
    ipc.send("detect_canny");
})
var watershed=document.getElementById("watershed");
watershed.addEventListener("onclick",function(e){
    ipc.send("watershed");
})
