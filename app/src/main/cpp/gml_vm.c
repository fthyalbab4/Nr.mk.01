#include "gml_vm.h"
#include "gm82_gml_builtins.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
static gml_value undef(void){gml_value v={0};return v;}
gml_value gml_value_real(double n){gml_value v=undef();v.kind=GML_V_REAL;v.real=n;return v;}
gml_value gml_value_bool(int b){gml_value v=undef();v.kind=GML_V_BOOL;v.boolean=!!b;v.real=!!b;return v;}
gml_value gml_value_string(const char*s){gml_value v=undef();v.kind=GML_V_STRING;const char*src=s?s:"";size_t n=strlen(src);v.string=malloc(n+1);if(v.string)memcpy(v.string,src,n+1);return v;} gml_value gml_value_array(size_t count){gml_value v=undef();v.kind=GML_V_ARRAY;v.array=calloc(1,sizeof *v.array);if(!v.array)return v;v.array->count=count;v.array->items=calloc(count?count:1,sizeof(gml_value));if(!v.array->items){free(v.array);v.array=0;v.kind=GML_V_UNDEFINED;}return v;} void gml_value_free(gml_value*v){if(!v)return;if(v->kind==GML_V_STRING)free(v->string);else if(v->kind==GML_V_ARRAY&&v->array){for(size_t i=0;i<v->array->count;i++)gml_value_free(&v->array->items[i]);free(v->array->items);free(v->array);}memset(v,0,sizeof*v);} static gml_value copyv(const gml_value*v){if(!v)return undef();if(v->kind==GML_V_STRING)return gml_value_string(v->string);if(v->kind==GML_V_ARRAY&&v->array){gml_value r=gml_value_array(v->array->count);if(r.array)for(size_t i=0;i<v->array->count;i++){r.array->items[i]=copyv(&v->array->items[i]);}return r;}return *v;}
void gml_vm_init(gml_vm*vm){memset(vm,0,sizeof*vm);}
void gml_vm_set_native_call(gml_vm*vm,gml_native_call callback,void*userdata){if(vm){vm->native_call=callback;vm->native_userdata=userdata;}} void gml_vm_set_name_resolver(gml_vm*vm,gml_name_resolve callback,void*userdata){if(vm){vm->name_resolve=callback;vm->name_userdata=userdata;}} void gml_vm_set_with_callback(gml_vm*vm,gml_with_call callback,void*userdata){if(vm){vm->with_call=callback;vm->with_userdata=userdata;}} void gml_vm_set_member_callbacks(gml_vm*vm,gml_member_get getter,gml_member_set setter,void*userdata){if(vm){vm->member_get=getter;vm->member_set=setter;vm->member_userdata=userdata;}} void gml_vm_set_script_call(gml_vm*vm,gml_script_call callback,void*userdata){if(vm){vm->script_call=callback;vm->script_userdata=userdata;}} void gml_vm_push_scope(gml_vm*vm){if(vm&&vm->scope_depth<GML_VM_MAX_SCOPE_DEPTH)vm->scope_marks[vm->scope_depth++]=vm->count;} void gml_vm_pop_scope(gml_vm*vm){if(!vm||!vm->scope_depth)return;size_t mark=vm->scope_marks[--vm->scope_depth];while(vm->count>mark){vm->count--;gml_value_free(&vm->vars[vm->count].value);memset(&vm->vars[vm->count],0,sizeof vm->vars[vm->count]);}}
int gml_vm_set(gml_vm*vm,const char*n,gml_value v){if(!vm||!n)return 0;size_t mark=vm->scope_depth?vm->scope_marks[vm->scope_depth-1]:0;for(size_t i=vm->count;i>mark;i--)if(!strcmp(vm->vars[i-1].name,n)){gml_value_free(&vm->vars[i-1].value);vm->vars[i-1].value=copyv(&v);return 1;}if(vm->count>=GML_VM_MAX_VARS)return 0;strncpy(vm->vars[vm->count].name,n,63);vm->vars[vm->count].name[63]=0;vm->vars[vm->count].value=copyv(&v);vm->count++;return 1;}
gml_value gml_vm_get(gml_vm*vm,const char*n){if(vm&&n)for(size_t i=vm->count;i>0;i--)if(!strcmp(vm->vars[i-1].name,n))return copyv(&vm->vars[i-1].value);return undef();}
static double num(gml_value v){if(v.kind==GML_V_BOOL)return v.boolean;if(v.kind==GML_V_REAL)return v.real;return 0;}
static int truth(gml_value v){if(v.kind==GML_V_STRING)return v.string&&v.string[0];return num(v)!=0;} static const char* text_of(gml_value v){return v.kind==GML_V_STRING&&v.string?v.string:"";} static gml_value number_text(double value){char buffer[64];snprintf(buffer,sizeof buffer,"%.15g",value);return gml_value_string(buffer);} static gml_value eval(gml_vm*vm,const gml_ast*n); static gml_value* named_slot(gml_vm*vm,const char*n){if(!vm||!n)return 0;for(size_t i=vm->count;i>0;i--)if(!strcmp(vm->vars[i-1].name,n))return &vm->vars[i-1].value;return 0;} static gml_value eval_member(gml_vm*vm,const gml_ast*n){if(!n||!n->left||!n->text)return undef();gml_value base=eval(vm,n->left);gml_value r=undef();if(!strcmp(n->text,"length")){if(base.kind==GML_V_ARRAY&&base.array)r=gml_value_real((double)base.array->count);else if(base.kind==GML_V_STRING&&base.string)r=gml_value_real((double)strlen(base.string));}else if(vm->member_get && n->left->kind==GML_AST_NAME && !strcmp(n->left->text,"self")){vm->member_get(vm->member_userdata,n->text,&r);}gml_value_free(&base);return r;} static gml_value eval_index(gml_vm*vm,const gml_ast*n){if(!n||!n->left||!n->right)return undef();gml_value*base=(n->left->kind==GML_AST_NAME)?named_slot(vm,n->left->text):0;gml_value temp=undef();if(!base){temp=eval(vm,n->left);base=&temp;}gml_value idx=eval(vm,n->right);size_t i=num(idx)<0?0:(size_t)num(idx);gml_value r=undef();if(base->kind==GML_V_ARRAY&&base->array&&i<base->array->count)r=copyv(&base->array->items[i]);gml_value_free(&idx);if(base==&temp)gml_value_free(&temp);return r;} static gml_value eval(gml_vm*vm,const gml_ast*n);
static gml_value call(gml_vm*vm,const gml_ast*n){if(!n->text)return undef();gml_value a[32];size_t c=n->count<32?n->count:32;for(size_t i=0;i<c;i++)a[i]=eval(vm,n->items[i]);gml_value r=undef();
if(!strcmp(n->text,"array_create")&&c==1){double raw=num(a[0]);size_t count=raw>0?(size_t)raw:0;if(count>4096)count=4096;r=gml_value_array(count);}
else if(!strcmp(n->text,"array_length_1d")&&c==1){r=gml_value_real((a[0].kind==GML_V_ARRAY&&a[0].array)?(double)a[0].array->count:0.0);}
else if(!strcmp(n->text,"array_length")&&c==1){r=gml_value_real((a[0].kind==GML_V_ARRAY&&a[0].array)?(double)a[0].array->count:0.0);}
else if(!strcmp(n->text,"sqrt")&&c==1)r=gml_value_real(sqrt(num(a[0])));
else if(!strcmp(n->text,"sqr")&&c==1){double v=num(a[0]);r=gml_value_real(v*v);}
else if(!strcmp(n->text,"frac")&&c==1){double v=num(a[0]);r=gml_value_real(v - (int)v);}
else if(!strcmp(n->text,"sin")&&c==1)r=gml_value_real(sin(num(a[0])));
else if(!strcmp(n->text,"cos")&&c==1)r=gml_value_real(cos(num(a[0])));
else if(!strcmp(n->text,"tan")&&c==1)r=gml_value_real(tan(num(a[0])));
else if(!strcmp(n->text,"arcsin")&&c==1)r=gml_value_real(asin(num(a[0])));
else if(!strcmp(n->text,"arccos")&&c==1)r=gml_value_real(acos(num(a[0])));
else if(!strcmp(n->text,"arctan")&&c==1)r=gml_value_real(atan(num(a[0])));
else if(!strcmp(n->text,"arctan2")&&c==2)r=gml_value_real(atan2(num(a[0]),num(a[1])));
else if(!strcmp(n->text,"exp")&&c==1)r=gml_value_real(exp(num(a[0])));
else if(!strcmp(n->text,"log2")&&c==1)r=gml_value_real(log2(num(a[0])));
else if(!strcmp(n->text,"log10")&&c==1)r=gml_value_real(log10(num(a[0])));
else if(!strcmp(n->text,"logn")&&c==2){double base=num(a[0]),v=num(a[1]);r=gml_value_real(base>0&&base!=1.0?log(v)/log(base):0.0);}
else if(!strcmp(n->text,"abs")&&c==1)r=gml_value_real(fabs(num(a[0])));
else if(!strcmp(n->text,"floor")&&c==1)r=gml_value_real(floor(num(a[0])));
else if(!strcmp(n->text,"ceil")&&c==1)r=gml_value_real(ceil(num(a[0])));
else if(!strcmp(n->text,"round")&&c==1)r=gml_value_real(round(num(a[0])));
else if(!strcmp(n->text,"sign")&&c==1)r=gml_value_real(num(a[0])>0?1:(num(a[0])<0?-1:0));
else if(!strcmp(n->text,"min")&&c>=1){double v=num(a[0]);for(size_t i=1;i<c;i++)if(num(a[i])<v)v=num(a[i]);r=gml_value_real(v);}
else if(!strcmp(n->text,"max")&&c>=1){double v=num(a[0]);for(size_t i=1;i<c;i++)if(num(a[i])>v)v=num(a[i]);r=gml_value_real(v);}
else if(!strcmp(n->text,"mean")&&c>=1){double sum=0;for(size_t i=0;i<c;i++)sum+=num(a[i]);r=gml_value_real(c>0?sum/(double)c:0.0);}
else if(!strcmp(n->text,"median")&&c==3){r=gml_value_real(gml_median(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"clamp")&&c==3){double x=num(a[0]),lo=num(a[1]),hi=num(a[2]);r=gml_value_real(x<lo?lo:(x>hi?hi:x));}
else if(!strcmp(n->text,"power")&&c==2)r=gml_value_real(pow(num(a[0]),num(a[1])));
else if(!strcmp(n->text,"degtorad")&&c==1)r=gml_value_real(num(a[0])*3.14159265358979323846/180.0);
else if(!strcmp(n->text,"radtodeg")&&c==1)r=gml_value_real(num(a[0])*180.0/3.14159265358979323846);
else if(!strcmp(n->text,"dsin")&&c==1)r=gml_value_real(gml_dsin(num(a[0])));
else if(!strcmp(n->text,"dcos")&&c==1)r=gml_value_real(gml_dcos(num(a[0])));
else if(!strcmp(n->text,"dtan")&&c==1)r=gml_value_real(gml_dtan(num(a[0])));
else if(!strcmp(n->text,"darcsin")&&c==1)r=gml_value_real(gml_darcsin(num(a[0])));
else if(!strcmp(n->text,"darccos")&&c==1)r=gml_value_real(gml_darccos(num(a[0])));
else if(!strcmp(n->text,"darctan")&&c==1)r=gml_value_real(gml_darctan(num(a[0])));
else if(!strcmp(n->text,"darctan2")&&c==2)r=gml_value_real(gml_darctan2(num(a[0]),num(a[1])));
else if(!strcmp(n->text,"lerp")&&c==3)r=gml_value_real(num(a[0])+(num(a[1])-num(a[0]))*num(a[2]));
else if(!strcmp(n->text,"random")&&c==1)r=gml_value_real(((double)rand()/(double)RAND_MAX)*num(a[0]));
else if(!strcmp(n->text,"random_range")&&c==2){double lo=num(a[0]),hi=num(a[1]);r=gml_value_real(lo+((double)rand()/(double)RAND_MAX)*(hi-lo));}
else if(!strcmp(n->text,"irandom")&&c==1){long max_val=(long)num(a[0]);r=gml_value_real(max_val>0?(double)(rand()%(max_val+1)):0.0);}
else if(!strcmp(n->text,"irandom_range")&&c==2){long lo=(long)num(a[0]),hi=(long)num(a[1]);if(hi<lo){long t=lo;lo=hi;hi=t;}long range=hi-lo+1;r=gml_value_real(range>0?(double)(lo+(rand()%range)):(double)lo);}
else if(!strcmp(n->text,"choose")&&c>=1){size_t pick=(size_t)(rand()%c);r=copyv(&a[pick]);}
else if(!strcmp(n->text,"point_distance")&&c==4){double dx=num(a[2])-num(a[0]),dy=num(a[3])-num(a[1]);r=gml_value_real(sqrt(dx*dx+dy*dy));}
else if(!strcmp(n->text,"point_direction")&&c==4){double dx=num(a[2])-num(a[0]),dy=num(a[1])-num(a[3]);double angle=atan2(dy,dx)*180.0/3.14159265358979323846;if(angle<0)angle+=360.0;r=gml_value_real(angle);}
else if(!strcmp(n->text,"lengthdir_x")&&c==2)r=gml_value_real(num(a[0])*cos(num(a[1])*3.14159265358979323846/180.0));
else if(!strcmp(n->text,"lengthdir_y")&&c==2)r=gml_value_real(-num(a[0])*sin(num(a[1])*3.14159265358979323846/180.0));
else if(!strcmp(n->text,"string")&&c==1){if(a[0].kind==GML_V_STRING)r=gml_value_string(text_of(a[0]));else if(a[0].kind==GML_V_BOOL)r=gml_value_string(a[0].boolean?"1":"0");else r=number_text(num(a[0]));}
else if(!strcmp(n->text,"string_length")&&c==1){r=gml_value_real((double)strlen(text_of(a[0])));}
else if(!strcmp(n->text,"string_char_at")&&c==2){const char*s=text_of(a[0]);int pos=(int)num(a[1]);if(pos>=1&&(size_t)pos<=strlen(s)){char ch[2]={s[pos-1],0};r=gml_value_string(ch);}else r=gml_value_string("");}
else if(!strcmp(n->text,"string_copy")&&c==3){const char*s=text_of(a[0]);int start=(int)num(a[1]),len=(int)num(a[2]);size_t nstr=strlen(s);if(start<1)start=1;if(len<0)len=0;size_t begin=(size_t)(start-1);if(begin>nstr)begin=nstr;if((size_t)len>nstr-begin)len=(int)(nstr-begin);char*part=malloc((size_t)len+1);if(part){memcpy(part,s+begin,(size_t)len);part[len]=0;r=gml_value_string(part);free(part);}}
else if(!strcmp(n->text,"string_pos")&&c==2){const char*needle=text_of(a[0]);const char*haystack=text_of(a[1]);const char*found=(needle[0]?strstr(haystack,needle):haystack);r=gml_value_real(found?(double)(found-haystack+1):0);}
else if(!strcmp(n->text,"string_lower")&&c==1){const char*s=text_of(a[0]);size_t len=strlen(s);char*buf=malloc(len+1);if(buf){for(size_t i=0;i<len;i++)buf[i]=(char)tolower((unsigned char)s[i]);buf[len]=0;r=gml_value_string(buf);free(buf);}}
else if(!strcmp(n->text,"string_upper")&&c==1){const char*s=text_of(a[0]);size_t len=strlen(s);char*buf=malloc(len+1);if(buf){for(size_t i=0;i<len;i++)buf[i]=(char)toupper((unsigned char)s[i]);buf[len]=0;r=gml_value_string(buf);free(buf);}}
else if(!strcmp(n->text,"string_delete")&&c==3){const char*s=text_of(a[0]);int start=(int)num(a[1]),del=(int)num(a[2]);size_t len=strlen(s);if(start<1)start=1;if(del<0)del=0;size_t begin=(size_t)(start-1);if(begin>len)begin=len;if((size_t)del>len-begin)del=(int)(len-begin);char*buf=malloc(len-(size_t)del+1);if(buf){memcpy(buf,s,begin);memcpy(buf+begin,s+begin+del,len-begin-(size_t)del);buf[len-(size_t)del]=0;r=gml_value_string(buf);free(buf);}}
else if(!strcmp(n->text,"string_insert")&&c==3){const char*ins=text_of(a[0]);const char*s=text_of(a[1]);int pos=(int)num(a[2]);size_t ni=strlen(ins),ns=strlen(s);if(pos<1)pos=1;size_t at=(size_t)(pos-1);if(at>ns)at=ns;char*buf=malloc(ni+ns+1);if(buf){memcpy(buf,s,at);memcpy(buf+at,ins,ni);memcpy(buf+at+ni,s+at,ns-at+1);r=gml_value_string(buf);free(buf);}}
else if(!strcmp(n->text,"string_repeat")&&c==2){const char*s=text_of(a[0]);int count=(int)num(a[1]);if(count<=0)r=gml_value_string("");else{if(count>1000)count=1000;size_t slen=strlen(s);char*buf=malloc(slen*count+1);if(buf){char*p=buf;for(int i=0;i<count;i++){memcpy(p,s,slen);p+=slen;}*p=0;r=gml_value_string(buf);free(buf);}}}
else if(!strcmp(n->text,"string_count")&&c==2){const char*substr=text_of(a[0]);const char*str=text_of(a[1]);size_t sublen=strlen(substr);double cnt=0;if(sublen>0){const char*p=str;while((p=strstr(p,substr))){cnt++;p+=sublen;}}r=gml_value_real(cnt);}
else if(!strcmp(n->text,"string_digits")&&c==1){const char*s=text_of(a[0]);size_t len=strlen(s);char*buf=malloc(len+1);if(buf){size_t out_idx=0;for(size_t i=0;i<len;i++)if(isdigit((unsigned char)s[i]))buf[out_idx++]=s[i];buf[out_idx]=0;r=gml_value_string(buf);free(buf);}}
else if(!strcmp(n->text,"string_letters")&&c==1){const char*s=text_of(a[0]);size_t len=strlen(s);char*buf=malloc(len+1);if(buf){size_t out_idx=0;for(size_t i=0;i<len;i++)if(isalpha((unsigned char)s[i]))buf[out_idx++]=s[i];buf[out_idx]=0;r=gml_value_string(buf);free(buf);}}
else if(!strcmp(n->text,"string_lettersdigits")&&c==1){char outbuf[512];gml_string_lettersdigits(text_of(a[0]),outbuf,sizeof outbuf);r=gml_value_string(outbuf);}
else if(!strcmp(n->text,"string_ord_at")&&c==2){r=gml_value_real(gml_string_ord_at(text_of(a[0]),num(a[1])));}
else if(!strcmp(n->text,"angle_difference")&&c==2){r=gml_value_real(gml_angle_difference(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"dot_product")&&c==4){r=gml_value_real(gml_dot_product(num(a[0]),num(a[1]),num(a[2]),num(a[3])));}
else if(!strcmp(n->text,"real")&&c==1){r=gml_value_real(a[0].kind==GML_V_STRING?atof(text_of(a[0])):num(a[0]));}
else if(!strcmp(n->text,"ord")&&c==1){const char*s=text_of(a[0]);r=gml_value_real(s&&s[0]?(double)(unsigned char)s[0]:0.0);}
else if(!strcmp(n->text,"chr")&&c==1){char ch[2]={(char)(unsigned char)num(a[0]),0};r=gml_value_string(ch);}
else if(!strcmp(n->text,"is_string")&&c==1){r=gml_value_bool(a[0].kind==GML_V_STRING);}
else if(!strcmp(n->text,"is_real")&&c==1){r=gml_value_bool(a[0].kind==GML_V_REAL);}
else if(!strcmp(n->text,"is_array")&&c==1){r=gml_value_bool(a[0].kind==GML_V_ARRAY);}
else if(!strcmp(n->text,"is_undefined")&&c==1){r=gml_value_bool(a[0].kind==GML_V_UNDEFINED);}
else if(!strcmp(n->text,"typeof")&&c==1){switch(a[0].kind){case GML_V_REAL:r=gml_value_string("number");break;case GML_V_BOOL:r=gml_value_string("bool");break;case GML_V_STRING:r=gml_value_string("string");break;case GML_V_ARRAY:r=gml_value_string("array");break;default:r=gml_value_string("undefined");break;}}
else if(!strcmp(n->text,"string_replace_all")&&c==3){const char*src=text_of(a[0]);const char*find=text_of(a[1]);const char*rep=text_of(a[2]);size_t nf=strlen(find),nr=strlen(rep),ns=strlen(src);if(nf==0)r=gml_value_string(src);else{size_t occurrences=0;for(const char*p=src;(p=strstr(p,find));p+=nf)occurrences++;size_t outlen=ns;if(nr>=nf)outlen += occurrences*(nr-nf);else outlen -= occurrences*(nf-nr);char*buf=malloc(outlen+1);if(buf){const char*p=src;char*w=buf;while(*p){const char*q=strstr(p,find);if(!q){strcpy(w,p);break;}size_t n=(size_t)(q-p);memcpy(w,p,n);w+=n;memcpy(w,rep,nr);w+=nr;p=q+nf;}buf[outlen]=0;r=gml_value_string(buf);free(buf);}}}
else if(!strcmp(n->text,"string_replace")&&c==3){
    const char*src=text_of(a[0]);
    const char*find=text_of(a[1]);
    const char*rep=text_of(a[2]);
    const char*p=strstr(src,find);
    if(!p||!find[0]){r=gml_value_string(src);}
    else{
        size_t pref=p-src,flen=strlen(find),rlen=strlen(rep),slen=strlen(src);
        size_t outlen=pref+rlen+(slen-pref-flen);
        char*buf=malloc(outlen+1);
        if(buf){
            memcpy(buf,src,pref);
            memcpy(buf+pref,rep,rlen);
            memcpy(buf+pref+rlen,p+flen,slen-pref-flen+1);
            r=gml_value_string(buf);
            free(buf);
        }else r=gml_value_string(src);
    }
}
else if(!strcmp(n->text,"string_byte_at")&&c==2){r=gml_value_real(gml_string_byte_at(text_of(a[0]),num(a[1])));}
else if(!strcmp(n->text,"string_byte_length")&&c==1){r=gml_value_real(gml_string_byte_length(text_of(a[0])));}
else if(!strcmp(n->text,"string_format")&&c==3){char out[256];gml_string_format(num(a[0]),num(a[1]),num(a[2]),out,sizeof out);r=gml_value_string(out);}
else if(!strcmp(n->text,"make_color_rgb")&&c==3){r=gml_value_real(gml_make_color_rgb(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"make_color_hsv")&&c==3){r=gml_value_real(gml_make_color_hsv(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"color_get_red")&&c==1){r=gml_value_real(gml_color_get_red(num(a[0])));}
else if(!strcmp(n->text,"color_get_green")&&c==1){r=gml_value_real(gml_color_get_green(num(a[0])));}
else if(!strcmp(n->text,"color_get_blue")&&c==1){r=gml_value_real(gml_color_get_blue(num(a[0])));}
else if(!strcmp(n->text,"color_get_hue")&&c==1){r=gml_value_real(gml_color_get_hue(num(a[0])));}
else if(!strcmp(n->text,"color_get_saturation")&&c==1){r=gml_value_real(gml_color_get_saturation(num(a[0])));}
else if(!strcmp(n->text,"color_get_value")&&c==1){r=gml_value_real(gml_color_get_value(num(a[0])));}
else if(!strcmp(n->text,"merge_color")&&c==3){r=gml_value_real(gml_merge_color(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"ds_list_create")&&c==0){r=gml_value_real(gml_ds_list_create());}
else if(!strcmp(n->text,"ds_list_destroy")&&c==1){r=gml_value_real(gml_ds_list_destroy(num(a[0])));}
else if(!strcmp(n->text,"ds_list_add")&&c==2){r=gml_value_real(gml_ds_list_add(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"ds_list_find_value")&&c==2){r=gml_value_real(gml_ds_list_find_value(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"ds_list_size")&&c==1){r=gml_value_real(gml_ds_list_size(num(a[0])));}
else if(!strcmp(n->text,"ds_list_clear")&&c==1){r=gml_value_real(gml_ds_list_clear(num(a[0])));}
else if(!strcmp(n->text,"ds_list_delete")&&c==2){r=gml_value_real(gml_ds_list_delete(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"ds_list_find_index")&&c==2){r=gml_value_real(gml_ds_list_find_index(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"ds_list_empty")&&c==1){r=gml_value_real(gml_ds_list_empty(num(a[0])));}
else if(!strcmp(n->text,"ds_map_create")&&c==0){r=gml_value_real(gml_ds_map_create());}
else if(!strcmp(n->text,"ds_map_destroy")&&c==1){r=gml_value_real(gml_ds_map_destroy(num(a[0])));}
else if(!strcmp(n->text,"ds_map_add")&&c==3){r=gml_value_real(gml_ds_map_add(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"ds_map_find_value")&&c==2){r=gml_value_real(gml_ds_map_find_value(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"ds_map_exists")&&c==2){r=gml_value_real(gml_ds_map_exists(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"ds_map_size")&&c==1){r=gml_value_real(gml_ds_map_size(num(a[0])));}
else if(!strcmp(n->text,"ds_map_clear")&&c==1){r=gml_value_real(gml_ds_map_clear(num(a[0])));}
else if(!strcmp(n->text,"ds_map_delete")&&c==2){r=gml_value_real(gml_ds_map_delete(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"ds_stack_create")&&c==0){r=gml_value_real(gml_ds_stack_create());}
else if(!strcmp(n->text,"ds_stack_destroy")&&c==1){r=gml_value_real(gml_ds_stack_destroy(num(a[0])));}
else if(!strcmp(n->text,"ds_stack_push")&&c==2){r=gml_value_real(gml_ds_stack_push(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"ds_stack_pop")&&c==1){r=gml_value_real(gml_ds_stack_pop(num(a[0])));}
else if(!strcmp(n->text,"ds_stack_top")&&c==1){r=gml_value_real(gml_ds_stack_top(num(a[0])));}
else if(!strcmp(n->text,"ds_stack_size")&&c==1){r=gml_value_real(gml_ds_stack_size(num(a[0])));}
else if(!strcmp(n->text,"ds_stack_empty")&&c==1){r=gml_value_real(gml_ds_stack_empty(num(a[0])));}
else if(!strcmp(n->text,"ds_queue_create")&&c==0){r=gml_value_real(gml_ds_queue_create());}
else if(!strcmp(n->text,"ds_queue_destroy")&&c==1){r=gml_value_real(gml_ds_queue_destroy(num(a[0])));}
else if(!strcmp(n->text,"ds_queue_enqueue")&&c==2){r=gml_value_real(gml_ds_queue_enqueue(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"ds_queue_dequeue")&&c==1){r=gml_value_real(gml_ds_queue_dequeue(num(a[0])));}
else if(!strcmp(n->text,"ds_queue_head")&&c==1){r=gml_value_real(gml_ds_queue_head(num(a[0])));}
else if(!strcmp(n->text,"ds_queue_tail")&&c==1){r=gml_value_real(gml_ds_queue_tail(num(a[0])));}
else if(!strcmp(n->text,"ds_queue_size")&&c==1){r=gml_value_real(gml_ds_queue_size(num(a[0])));}
else if(!strcmp(n->text,"ds_queue_empty")&&c==1){r=gml_value_real(gml_ds_queue_empty(num(a[0])));}
else if(!strcmp(n->text,"ds_queue_clear")&&c==1){r=gml_value_real(gml_ds_queue_clear(num(a[0])));}
else if(!strcmp(n->text,"ds_priority_create")&&c==0){r=gml_value_real(gml_ds_priority_create());}
else if(!strcmp(n->text,"ds_priority_destroy")&&c==1){r=gml_value_real(gml_ds_priority_destroy(num(a[0])));}
else if(!strcmp(n->text,"ds_priority_add")&&c==3){r=gml_value_real(gml_ds_priority_add(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"ds_priority_find_max")&&c==1){r=gml_value_real(gml_ds_priority_find_max(num(a[0])));}
else if(!strcmp(n->text,"ds_priority_delete_max")&&c==1){r=gml_value_real(gml_ds_priority_delete_max(num(a[0])));}
else if(!strcmp(n->text,"ds_priority_size")&&c==1){r=gml_value_real(gml_ds_priority_size(num(a[0])));}
else if(!strcmp(n->text,"ds_priority_empty")&&c==1){r=gml_value_real(gml_ds_priority_empty(num(a[0])));}
else if(!strcmp(n->text,"ds_grid_create")&&c==2){r=gml_value_real(gml_ds_grid_create(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"ds_grid_destroy")&&c==1){r=gml_value_real(gml_ds_grid_destroy(num(a[0])));}
else if(!strcmp(n->text,"ds_grid_width")&&c==1){r=gml_value_real(gml_ds_grid_width(num(a[0])));}
else if(!strcmp(n->text,"ds_grid_height")&&c==1){r=gml_value_real(gml_ds_grid_height(num(a[0])));}
else if(!strcmp(n->text,"ds_grid_set")&&c==4){r=gml_value_real(gml_ds_grid_set(num(a[0]),num(a[1]),num(a[2]),num(a[3])));}
else if(!strcmp(n->text,"ds_grid_get")&&c==3){r=gml_value_real(gml_ds_grid_get(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"ds_grid_clear")&&c==2){r=gml_value_real(gml_ds_grid_clear(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"ds_grid_get_sum")&&c==5){r=gml_value_real(gml_ds_grid_get_sum(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4])));}
else if(!strcmp(n->text,"ds_grid_get_max")&&c==5){r=gml_value_real(gml_ds_grid_get_max(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4])));}
else if(!strcmp(n->text,"ds_grid_get_min")&&c==5){r=gml_value_real(gml_ds_grid_get_min(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4])));}
else if(!strcmp(n->text,"ds_grid_get_mean")&&c==5){r=gml_value_real(gml_ds_grid_get_mean(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4])));}
else if(!strcmp(n->text,"draw_self")&&c==0){gml_draw_self();r=gml_value_real(1);}
else if(!strcmp(n->text,"draw_sprite_ext")&&c==9){gml_draw_sprite_ext(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4]),num(a[5]),num(a[6]),num(a[7]),num(a[8]));r=gml_value_real(1);}
else if(!strcmp(n->text,"move_snap")&&c==2){r=gml_value_real(gml_move_snap(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"place_snapped")&&c==2){r=gml_value_real(gml_place_snapped(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"instance_position")&&c==3){r=gml_value_real(gml_instance_position(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"instance_find")&&c==2){r=gml_value_real(gml_instance_find(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"instance_number")&&c==1){r=gml_value_real(gml_instance_number(num(a[0])));}
else if(!strcmp(n->text,"array_length_2d")&&c==2){r=gml_value_real(gml_array_length_1d(num(a[0])));}
else if(!strcmp(n->text,"array_height_2d")&&c==1){r=gml_value_real(gml_array_height_2d(num(a[0])));}
else if(!strcmp(n->text,"ini_open")&&c==1){r=gml_value_real(gml_ini_open(text_of(a[0])));}
else if(!strcmp(n->text,"ini_close")&&c==0){r=gml_value_real(gml_ini_close());}
else if(!strcmp(n->text,"ini_read_real")&&c==3){r=gml_value_real(gml_ini_read_real(text_of(a[0]),text_of(a[1]),num(a[2])));}
else if(!strcmp(n->text,"ini_write_real")&&c==3){r=gml_value_real(gml_ini_write_real(text_of(a[0]),text_of(a[1]),num(a[2])));}
else if(!strcmp(n->text,"ini_key_exists")&&c==2){r=gml_value_real(gml_ini_key_exists(text_of(a[0]),text_of(a[1])));}
else if(!strcmp(n->text,"file_exists")&&c==1){r=gml_value_real(gml_file_exists(text_of(a[0])));}
else if(!strcmp(n->text,"file_delete")&&c==1){r=gml_value_real(gml_file_delete(text_of(a[0])));}
else if(!strcmp(n->text,"directory_exists")&&c==1){r=gml_value_real(gml_directory_exists(text_of(a[0])));}
else if(!strcmp(n->text,"buffer_create")&&c>=1){r=gml_value_real(gml_buffer_create(num(a[0]),c>=2?num(a[1]):0,c>=3?num(a[2]):1));}
else if(!strcmp(n->text,"buffer_delete")&&c==1){r=gml_value_real(gml_buffer_delete(num(a[0])));}
else if(!strcmp(n->text,"buffer_write")&&c==3){r=gml_value_real(gml_buffer_write(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"buffer_read")&&c==2){r=gml_value_real(gml_buffer_read(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"buffer_poke")&&c==4){r=gml_value_real(gml_buffer_poke(num(a[0]),num(a[1]),num(a[2]),num(a[3])));}
else if(!strcmp(n->text,"buffer_peek")&&c==3){r=gml_value_real(gml_buffer_peek(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"directory_create")&&c==1){r=gml_value_real(gml_directory_create(text_of(a[0])));}
else if(!strcmp(n->text,"file_copy")&&c==2){r=gml_value_real(gml_file_copy(text_of(a[0]),text_of(a[1])));}
else if(!strcmp(n->text,"file_move")&&c==2){r=gml_value_real(gml_file_move(text_of(a[0]),text_of(a[1])));}
else if(!strcmp(n->text,"buffer_seek")&&c==3){r=gml_value_real(gml_buffer_seek(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"buffer_tell")&&c==1){r=gml_value_real(gml_buffer_tell(num(a[0])));}
else if(!strcmp(n->text,"buffer_get_size")&&c==1){r=gml_value_real(gml_buffer_get_size(num(a[0])));}
else if(!strcmp(n->text,"buffer_sizeof")&&c==1){r=gml_value_real(gml_buffer_sizeof(num(a[0])));}
else if(!strcmp(n->text,"collision_circle")&&c==6){r=gml_value_real(gml_collision_circle(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4]),num(a[5])));}
else if(!strcmp(n->text,"collision_rectangle")&&c==7){r=gml_value_real(gml_collision_rectangle(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4]),num(a[5]),num(a[6])));}
else if(!strcmp(n->text,"collision_line")&&c==7){r=gml_value_real(gml_collision_line(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4]),num(a[5]),num(a[6])));}
else if(!strcmp(n->text,"collision_ellipse")&&c==7){r=gml_value_real(gml_collision_ellipse(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4]),num(a[5]),num(a[6])));}
else if(!strcmp(n->text,"collision_point")&&c==5){r=gml_value_real(gml_collision_point(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4])));}
else if(!strcmp(n->text,"place_meeting")&&c==3){r=gml_value_real(gml_place_meeting(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"place_free")&&c==2){r=gml_value_real(gml_place_free(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"keyboard_check_direct")&&c==1){r=gml_value_real(gml_keyboard_check_direct(num(a[0])));}
else if(!strcmp(n->text,"keyboard_clear")&&c==1){r=gml_value_real(gml_keyboard_clear(num(a[0])));}
else if(!strcmp(n->text,"io_clear")&&c==0){r=gml_value_real(gml_io_clear());}
else if(!strcmp(n->text,"sound_volume")&&c==2){r=gml_value_real(gml_sound_volume(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"sound_pan")&&c==2){r=gml_value_real(gml_sound_pan(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"sound_pitch")&&c==2){r=gml_value_real(gml_sound_pitch(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"instance_change")&&c==2){r=gml_value_real(gml_instance_change(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"instance_copy")&&c==1){r=gml_value_real(gml_instance_copy(num(a[0])));}
else if(!strcmp(n->text,"instance_deactivate_all")&&c==1){r=gml_value_real(gml_instance_deactivate_all(num(a[0])));}
else if(!strcmp(n->text,"instance_activate_all")&&c==0){r=gml_value_real(gml_instance_activate_all());}
else if(!strcmp(n->text,"file_find_first")&&c==2){r=gml_value_string(gml_file_find_first(text_of(a[0]),num(a[1])));}
else if(!strcmp(n->text,"file_find_next")&&c==0){r=gml_value_string(gml_file_find_next());}
else if(!strcmp(n->text,"file_find_close")&&c==0){gml_file_find_close();r=gml_value_real(0);}
else if(!strcmp(n->text,"point_distance_3d")&&c==6){r=gml_value_real(gml_point_distance_3d(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4]),num(a[5])));}
else if(!strcmp(n->text,"dot_product_3d")&&c==6){r=gml_value_real(gml_dot_product_3d(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4]),num(a[5])));}
else if(!strcmp(n->text,"ds_list_insert")&&c==3){r=gml_value_real(gml_ds_list_insert(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"ds_list_replace")&&c==3){r=gml_value_real(gml_ds_list_replace(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"ds_map_replace")&&c==3){r=gml_value_real(gml_ds_map_replace(num(a[0]),num(a[1]),num(a[2])));}
else if(!strcmp(n->text,"ds_grid_add")&&c==4){r=gml_value_real(gml_ds_grid_add(num(a[0]),num(a[1]),num(a[2]),num(a[3])));}
else if(!strcmp(n->text,"ds_grid_multiply")&&c==4){r=gml_value_real(gml_ds_grid_multiply(num(a[0]),num(a[1]),num(a[2]),num(a[3])));}
else if(!strcmp(n->text,"string_trim")&&c==1){char outbuf[512];gml_string_trim(text_of(a[0]),outbuf,sizeof outbuf);r=gml_value_string(outbuf);}
else if(!strcmp(n->text,"instance_deactivate_object")&&c==1){r=gml_value_real(gml_instance_deactivate_object(num(a[0])));}
else if(!strcmp(n->text,"instance_activate_object")&&c==1){r=gml_value_real(gml_instance_activate_object(num(a[0])));}
else if(!strcmp(n->text,"ds_grid_set_region")&&c==6){r=gml_value_real(gml_ds_grid_set_region(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4]),num(a[5])));}
else if(!strcmp(n->text,"ds_grid_fill")&&c==2){r=gml_value_real(gml_ds_grid_fill(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"ds_list_sort")&&c==2){r=gml_value_real(gml_ds_list_sort(num(a[0]),num(a[1])));}
else if(!strcmp(n->text,"ds_list_shuffle")&&c==1){r=gml_value_real(gml_ds_list_shuffle(num(a[0])));}
else if(!strcmp(n->text,"string_pos_ext")&&c==3){r=gml_value_real(gml_string_pos_ext(text_of(a[0]),text_of(a[1]),num(a[2])));}
else if(!strcmp(n->text,"string_last_pos")&&c==2){r=gml_value_real(gml_string_last_pos(text_of(a[0]),text_of(a[1])));}
else if(!strcmp(n->text,"instance_deactivate_region")&&c==6){r=gml_value_real(gml_instance_deactivate_region(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4]),num(a[5])));}
else if(!strcmp(n->text,"instance_activate_region")&&c==5){r=gml_value_real(gml_instance_activate_region(num(a[0]),num(a[1]),num(a[2]),num(a[3]),num(a[4])));}
else if(vm->native_call && vm->native_call(vm->native_userdata,n->text,a,c,&r)){}
else if(vm->script_call && vm->script_call(vm->script_userdata,n->text,a,c,&r)){}
else snprintf(vm->error,sizeof vm->error,"unknown function: %s",n->text);
for(size_t i=0;i<c;i++)gml_value_free(&a[i]);return r;}
static gml_value eval(gml_vm*vm,const gml_ast*n){if(!n)return undef();switch(n->kind){case GML_AST_NUMBER:return gml_value_real(n->number);case GML_AST_STRING:return gml_value_string(n->text);case GML_AST_NAME:{gml_value named=gml_vm_get(vm,n->text);if(named.kind==GML_V_UNDEFINED&&vm->name_resolve){gml_value resolved=undef();if(vm->name_resolve(vm->name_userdata,n->text,&resolved)){gml_value_free(&named);return resolved;}gml_value_free(&resolved);}return named;}case GML_AST_INDEX:return eval_index(vm,n);case GML_AST_MEMBER:return eval_member(vm,n);case GML_AST_CALL:return call(vm,n);case GML_AST_TERNARY:{gml_value condition=eval(vm,n->left);int choose_yes=truth(condition);gml_value_free(&condition);return eval(vm,choose_yes?n->right:n->items[0]);}case GML_AST_UNARY:{gml_value a=eval(vm,n->left);double x=num(a);gml_value r=(n->op==GML_T_NOT)?gml_value_bool(!truth(a)):gml_value_real(n->op==GML_T_MINUS?-x:x);gml_value_free(&a);return r;}case GML_AST_ASSIGN:{gml_value r=eval(vm,n->right);if(n->left&&n->left->kind==GML_AST_NAME)gml_vm_set(vm,n->left->text,r);else if(n->left&&n->left->kind==GML_AST_MEMBER&&n->left->left&&n->left->left->kind==GML_AST_NAME&&!strcmp(n->left->left->text,"self")){if(vm->member_set)vm->member_set(vm->member_userdata,n->left->text,&r);}else if(n->left&&n->left->kind==GML_AST_INDEX&&n->left->left&&n->left->left->kind==GML_AST_NAME){gml_value*base=named_slot(vm,n->left->left->text);gml_value idx=eval(vm,n->left->right);size_t i=num(idx)<0?0:(size_t)num(idx);if(base&&base->kind==GML_V_ARRAY&&base->array&&i<base->array->count){gml_value_free(&base->array->items[i]);base->array->items[i]=copyv(&r);}gml_value_free(&idx);}return r;}case GML_AST_BINARY:{
 gml_value a=eval(vm,n->left);
 if(n->op==GML_T_AND){
  int left_truth=truth(a);
  gml_value_free(&a);
  if(!left_truth)return gml_value_bool(0);
  gml_value b=eval(vm,n->right);
  int right_truth=truth(b);
  gml_value_free(&b);
  return gml_value_bool(right_truth);
 }
 if(n->op==GML_T_OR){
  int left_truth=truth(a);
  gml_value_free(&a);
  if(left_truth)return gml_value_bool(1);
  gml_value b=eval(vm,n->right);
  int right_truth=truth(b);
  gml_value_free(&b);
  return gml_value_bool(right_truth);
 }
 gml_value b=eval(vm,n->right);double x=num(a),y=num(b),z=0;int bo=0;gml_value r=undef();switch(n->op){case GML_T_PLUS:if(a.kind==GML_V_STRING||b.kind==GML_V_STRING){const char*as=a.kind==GML_V_STRING?(a.string?a.string:""):"";const char*bs=b.kind==GML_V_STRING?(b.string?b.string:""):"";size_t na=strlen(as),nb=strlen(bs);char*joined=malloc(na+nb+1);if(joined){memcpy(joined,as,na);memcpy(joined+na,bs,nb+1);r=gml_value_string(joined);free(joined);}else r=undef();}else z=x+y;break;case GML_T_MINUS:z=x-y;break;case GML_T_STAR:z=x*y;break;case GML_T_SLASH:z=y==0?0:x/y;break;case GML_T_PERCENT:z=fmod(x,y);break;case GML_T_EQ:bo=(a.kind==GML_V_STRING||b.kind==GML_V_STRING)?!strcmp(a.string?a.string:"",b.string?b.string:""):x==y;break;case GML_T_NE:bo=(a.kind==GML_V_STRING||b.kind==GML_V_STRING)?strcmp(a.string?a.string:"",b.string?b.string:"")!=0:x!=y;break;case GML_T_LT:bo=x<y;break;case GML_T_LE:bo=x<=y;break;case GML_T_GT:bo=x>y;break;case GML_T_GE:bo=x>=y;break;case GML_T_AND:bo=truth(a)&&truth(b);break;case GML_T_OR:bo=truth(a)||truth(b);break;default:break;}r=(n->op==GML_T_PLUS&&(a.kind==GML_V_STRING||b.kind==GML_V_STRING))?r:(((n->op>=GML_T_EQ&&n->op<=GML_T_GE)||n->op==GML_T_AND||n->op==GML_T_OR)?gml_value_bool(bo):gml_value_real(z));gml_value_free(&a);gml_value_free(&b);return r;}default:return undef();}}
static void exec(gml_vm*vm,const gml_ast*n){if(!n||vm->returned||vm->error[0])return;switch(n->kind){case GML_AST_BLOCK:for(size_t i=0;i<n->count&&!vm->returned&&!vm->break_pending&&!vm->continue_pending&&!vm->error[0];i++)exec(vm,n->items[i]);break;case GML_AST_EXPR_STMT:{gml_value v=eval(vm,n->left);gml_value_free(&v);break;}case GML_AST_RETURN:vm->return_value=eval(vm,n->left);vm->returned=1;break;case GML_AST_EXIT:gml_value_free(&vm->return_value);vm->return_value=undef();vm->returned=1;break;case GML_AST_BREAK:vm->break_pending=1;break;case GML_AST_CONTINUE:vm->continue_pending=1;break;case GML_AST_IF:{gml_value c=eval(vm,n->left);if(truth(c))exec(vm,n->right);else if(n->count)exec(vm,n->items[0]);gml_value_free(&c);break;}case GML_AST_WHILE:{size_t guard=0;while(!vm->returned&&!vm->error[0]&&!vm->break_pending&&guard++<100000){gml_value c=eval(vm,n->left);int ok=truth(c);gml_value_free(&c);if(!ok)break;vm->continue_pending=0;exec(vm,n->right);if(vm->break_pending){vm->break_pending=0;break;}if(vm->continue_pending){vm->continue_pending=0;continue;}}if(guard>=100000&&!vm->returned)snprintf(vm->error,sizeof vm->error,"while loop limit exceeded");break;}case GML_AST_DO_UNTIL:{size_t guard=0;do{if(vm->returned||vm->error[0])break;vm->continue_pending=0;exec(vm,n->right);if(vm->break_pending){vm->break_pending=0;break;}if(vm->continue_pending)vm->continue_pending=0;gml_value c=eval(vm,n->left);int done=truth(c);gml_value_free(&c);if(done)break;}while(++guard<100000);if(guard>=100000&&!vm->returned&&!vm->error[0])snprintf(vm->error,sizeof vm->error,"do-until loop limit exceeded");break;}case GML_AST_SWITCH:{gml_value key=eval(vm,n->left);size_t match=(size_t)-1,def=(size_t)-1;for(size_t i=0;i<n->count;i++){const gml_ast*c=n->items[i];if(!c||c->kind!=GML_AST_SWITCH_CASE)continue;if(!c->left){def=i;continue;}gml_value cv=eval(vm,c->left);int same=(key.kind==GML_V_STRING||cv.kind==GML_V_STRING)?!strcmp(key.string?key.string:"",cv.string?cv.string:""):num(key)==num(cv);gml_value_free(&cv);if(same){match=i;break;}}if(match==(size_t)-1)match=def;if(match!=(size_t)-1){for(size_t i=match;i<n->count&&!vm->returned&&!vm->error[0];i++){const gml_ast*c=n->items[i];if(!c||c->kind!=GML_AST_SWITCH_CASE)continue;vm->continue_pending=0;exec(vm,c->right);if(vm->break_pending){vm->break_pending=0;break;}}}gml_value_free(&key);break;}case GML_AST_FOR:{if(n->count<4)break;gml_value init=eval(vm,n->items[0]);gml_value_free(&init);size_t guard=0;while(!vm->returned&&!vm->error[0]&&!vm->break_pending&&guard++<100000){gml_value c=eval(vm,n->items[1]);int ok=truth(c);gml_value_free(&c);if(!ok)break;vm->continue_pending=0;exec(vm,n->items[3]);if(vm->break_pending){vm->break_pending=0;break;}vm->continue_pending=0;gml_value post=eval(vm,n->items[2]);gml_value_free(&post);}if(guard>=100000&&!vm->returned)snprintf(vm->error,sizeof vm->error,"for loop limit exceeded");break;}case GML_AST_WITH:{gml_value target=eval(vm,n->left);if(vm->with_call)vm->with_call(vm->with_userdata,vm,&target,n->right);else snprintf(vm->error,sizeof vm->error,"with callback unavailable");gml_value_free(&target);break;}case GML_AST_REPEAT:{gml_value count=eval(vm,n->left);double raw=num(count);gml_value_free(&count);size_t limit=raw>0?(size_t)raw:0;if(limit>100000)limit=100000;for(size_t i=0;i<limit&&!vm->returned&&!vm->error[0];i++){vm->break_pending=0;vm->continue_pending=0;exec(vm,n->right);if(vm->break_pending){vm->break_pending=0;break;}if(vm->continue_pending)vm->continue_pending=0;}break;}default:break;}}
int gml_vm_execute(gml_vm*vm,const gml_ast*root){if(!vm||!root)return 0;vm->error[0]=0;vm->returned=0;vm->break_pending=0;vm->continue_pending=0;gml_value_free(&vm->return_value);exec(vm,root);return vm->error[0]==0;} int gml_vm_invoke(gml_vm*vm,const gml_ast*root,const gml_value*args,size_t count,gml_value*out){if(!vm||!root)return 0;int old_returned=vm->returned,old_break=vm->break_pending,old_continue=vm->continue_pending;gml_value old_return=copyv(&vm->return_value);char old_error[160];memcpy(old_error,vm->error,sizeof old_error);vm->returned=0;vm->break_pending=0;vm->continue_pending=0;vm->error[0]=0;gml_vm_push_scope(vm);for(size_t i=0;i<count&&i<16;i++){char name[16];snprintf(name,sizeof name,"arg%zu",i);gml_vm_set(vm,name,args[i]);}gml_value_free(&vm->return_value);vm->return_value=undef();exec(vm,root);if(out)*out=copyv(&vm->return_value);int ok=vm->error[0]==0;gml_vm_pop_scope(vm);gml_value_free(&vm->return_value);vm->return_value=old_return;vm->returned=old_returned;vm->break_pending=old_break;vm->continue_pending=old_continue;if(!ok)memcpy(old_error,vm->error,sizeof old_error);memcpy(vm->error,old_error,sizeof vm->error);return ok;}
