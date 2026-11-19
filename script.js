var $ = function(id){return document.getElementById(id)};
var hitung, konversi, getAns, resetAns, satNama, riwayatJumlah, riwayatAmbil, hapusRiwayat;
var anim = !!window.gsap && !window.matchMedia('(prefers-reduced-motion: reduce)').matches;

var Module = {
  onRuntimeInitialized: function(){
    hitung = Module.cwrap('hitung','string',['string']);
    konversi = Module.cwrap('konversi','string',['number','number','number','number','number']);
    getAns = Module.cwrap('get_ans','number',[]);
    resetAns = Module.cwrap('reset_ans',null,[]);
    satNama = Module.cwrap('satuan_nama','string',['number','number']);
    riwayatJumlah = Module.cwrap('riwayat_jumlah','number',[]);
    riwayatAmbil = Module.cwrap('riwayat_ambil','string',['number']);
    hapusRiwayat = Module.cwrap('hapus_riwayat',null,[]);
    isiSatuan();
  }
};

function masuk(pane){
  if(!anim || pane.id==='kal')return;
  var id='#'+pane.id;
  gsap.fromTo(
    id+' .section-title,'+id+' .section-desc,'+id+' .field,'+id+' .btn,'+id+' .result-box,'+id+' .history-list li,'+id+' .empty',
    {y:12,opacity:0},
    {y:0,opacity:1,duration:.35,stagger:.05,ease:'power2.out',clearProps:'opacity,transform'}
  );
}

document.querySelectorAll('.tabs button').forEach(function(b){
  b.onclick=function(){
    var current=document.querySelector('.pane.on');
    var next=$(b.dataset.t);
    if(current===next)return;
    document.querySelectorAll('.tabs button').forEach(function(x){x.classList.remove('on')});
    b.classList.add('on');
    function tampil(){
      current.classList.remove('on');
      next.classList.add('on');
      if(b.dataset.t==='rwy')tampilRiwayat();
      masuk(next);
    }
    if(anim && current.id!=='kal'){
      gsap.to(current,{opacity:0,y:7,duration:.13,onComplete:function(){
        gsap.set(current,{clearProps:'opacity,transform'});
        tampil();
      }});
    }else tampil();
  };
});

var keys=[
  {v:'(',c:'op'},{v:')',c:'op'},{v:'^',c:'op'},{v:'√',c:'op'},
  {v:'7'},{v:'8'},{v:'9'},{v:'/',c:'op'},
  {v:'4'},{v:'5'},{v:'6'},{v:'*',c:'op'},
  {v:'1'},{v:'2'},{v:'3'},{v:'-',c:'op'},
  {v:'0'},{v:'.'},{v:'Ans',c:'op'},{v:'+',c:'op'},
  {v:'C',c:'utility'},{v:'⌫',c:'utility'},{v:'=',c:'eq'}
];
keys.forEach(function(k){
  var b=document.createElement('button');
  b.textContent=k.v;
  if(k.c)b.className=k.c;
  b.type='button';
  b.onclick=function(){
    if(k.v==='='){hit();return}
    if(k.v==='C')$('in').value='';
    else if(k.v==='⌫')$('in').value=$('in').value.slice(0,-1);
    else $('in').value+=k.v;
    $('in').focus();
  };
  $('pad').appendChild(b);
});
$('in').addEventListener('keydown',function(e){if(e.key==='Enter'){e.preventDefault();hit()}});

$('resetAnsBtn').onclick=function(){
  if(!resetAns){$('res').innerHTML='<span class="er">Modul C++ belum memuat fungsi reset Ans.</span>';return}
  resetAns();
  $('ansv').textContent='0.00';
};

function ready(){
  if(!hitung){$('res').innerHTML='<span class="er">Modul C++ (calc.js) belum dimuat.</span>';return false}
  return true;
}

function hit(){
  if(!ready())return;
  var ex=$('in').value.trim();if(!ex)return;
  var r=hitung(ex);
  if(r.indexOf('OK:')===0){
    $('res').innerHTML='<span class="ok">= '+r.slice(3)+'</span>';
    $('ansv').textContent=getAns().toFixed(2);
  }else $('res').innerHTML='<span class="er">'+r.slice(4)+'</span>';
}

function isiSatuan(){
  var k=+$('kat').value,a=$('asal'),t=$('tuj');
  a.innerHTML=t.innerHTML='';
  if(k===3){$('satBox').style.display='none';$('suhuBox').style.display='block';return}
  $('satBox').style.display='block';$('suhuBox').style.display='none';
  for(var i=0;i<7;i++){
    var n=satNama(k,i);if(!n)continue;
    a.add(new Option(n,i));t.add(new Option(n,i));
  }
  if(t.options.length>1)t.selectedIndex=1;
}

$('kat').onchange=function(){
  if(!satNama)return;
  isiSatuan();
  if(anim){
    var box=+$('kat').value===3?'#suhuBox':'#satBox';
    gsap.fromTo(box+' .field',{y:8,opacity:0},{y:0,opacity:1,duration:.3,stagger:.06,ease:'power2.out',clearProps:'opacity,transform'});
  }
};

function gagal(el){
  if(anim)gsap.fromTo(el,{x:-6},{x:0,duration:.45,ease:'elastic.out(1,.3)',clearProps:'transform'});
}

$('goKon').onclick=function(){
  if(!konversi){$('resK').innerHTML='<span class="er">Modul C++ belum dimuat.</span>';return}
  var v=parseFloat($('nilai').value);
  if(isNaN(v)){$('resK').innerHTML='<span class="er">Nilai tidak valid!</span>';gagal('#resK');return}
  var r=konversi(+$('kat').value,+$('asal').value||0,+$('tuj').value||0,v,+$('mode').value);
  if(r.indexOf('OK:')===0){
    $('resK').innerHTML='<span class="ok">'+r.slice(3)+'</span>';
    $('ansv').textContent=getAns().toFixed(2);
    if(anim)gsap.fromTo('#resK',{scale:.96,opacity:.4},{scale:1,opacity:1,duration:.35,ease:'back.out(2)',clearProps:'transform,opacity'});
  }else{
    $('resK').innerHTML='<span class="er">'+r.slice(4)+'</span>';
    gagal('#resK');
  }
};

function tampilRiwayat(){
  var n=riwayatJumlah?riwayatJumlah():0,items=[];
  for(var i=0;i<n;i++)items.push(riwayatAmbil(i));
  $('hist').innerHTML=items.length
    ?'<ol class="history-list">'+items.map(function(h){return '<li>'+h.replace(/&/g,'&amp;').replace(/</g,'&lt;')+'</li>'}).join('')+'</ol>'
    :'<div class="empty">Belum ada riwayat perhitungan.</div>';
}

$('clr').onclick=function(){
  function hapus(){if(hapusRiwayat)hapusRiwayat();tampilRiwayat();if(anim)gsap.from('#hist .empty',{opacity:0,y:8,duration:.3,clearProps:'opacity,transform'})}
  var li=document.querySelectorAll('#hist li');
  if(anim && li.length)gsap.to(li,{opacity:0,x:20,duration:.2,stagger:.03,onComplete:hapus});
  else hapus();
};

if(anim){
  gsap.from('.eyebrow,.team h1',{y:-12,opacity:0,duration:.55,stagger:.08,ease:'power3.out',clearProps:'opacity,transform'});
  gsap.from('.member',{x:-16,opacity:0,duration:.45,stagger:.06,delay:.2,ease:'power2.out',clearProps:'opacity,transform'});
  gsap.from('.topbar,.tabs',{y:14,opacity:0,duration:.5,stagger:.1,delay:.35,ease:'power3.out',clearProps:'opacity,transform'});
  gsap.from('.brand-mark',{rotate:-90,scale:.6,duration:.6,delay:.4,ease:'back.out(2)',clearProps:'transform'});
  gsap.from('.footer',{opacity:0,duration:.6,delay:.8,clearProps:'opacity'});
}
