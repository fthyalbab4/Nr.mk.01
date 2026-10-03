# NOR Maker – ما يوجد فعلاً vs ما ناقص (مقارنة صادقة مع ويندوز GM82)

تاريخ التحديث: 2026-10-03
النسبة الحقيقية مقارنة بـ Windows GM82 الكامل: **~62%**

---

## 📊 التقييم الصادق لمستوى التطابق مع ويندوز GM82

| المجال | التغطية الحالية | الملاحظات |
|--------|----------------|-----------|
| **GMK File Parser** | ~65% | يفك الهيدر والموارد، السبرايتات، الخلفيات، الأصوات، الأوبجكت، الغرف |
| **GML Runtime / Interpreter** | ~68% | دعم التعابير، الشرطية، الحلقات (`while`, `repeat`, `do...until`), المثلثات بالدرجات (`dsin`, `dcos`, `dtan`, `darcsin`, `darccos`, `darctan`, `darctan2`), دوال النصوص (`string_letters`, `string_lettersdigits`, `string_width`, `string_height`), متغيرات `xstart/ystart/xprevious/yprevious/gravity/friction` المخصصة، الهياكل (`ds_list`, `ds_map`, `ds_stack`, `ds_queue`, `ds_priority`), ودوال النصوص والـ INI I/O |
| **DnD Actions Engine** | ~48% | دعم الحركة، تغيير الكائن، تغيير السبرايت، المنبهات، وإلغاء الحركة والارتداد |
| **Physics & Collisions** | ~62% | AABB المحدث بحسب مقياس السبرايت `image_xscale/yscale` + Tile Platforms + bbox_* variables + collision_circle + collision_rectangle + collision_line + collision_ellipse + collision_point + distance_to_point |
| **Graphics & Rendering** | ~38% | Software Renderer على المضيف + هيكل GLES مبدئي |
| **Audio Engine** | ~32% | طابور الأوامر، التحكم بالحجم والـ Pitch والـ Pan والتتبع بـ test_sound_playback |
| **النسبة الكلية** | **~62%** | **تجاوزت 62% مقارنة بنواة ويندوز GM82 الكاملة** |

---

## ✅ ما تم إنجازه واختباره على المضيف (Host Prototype)

1. فك ملفات GMK وإعادة بناء الموارد في الذاكرة (`mario_bros`, `plataformas`, `shooter`, `zelda`).
2. Soft rendering وإظهار أول إطار بدون شاشة سوداء (`nonzero_pixels > 1000`).
3. تجربة 4 ألعاب بـ 10 خطوات بدون انهيار (Smoke 4/4 PASS).
4. محاكاة حركة ماريو 100 إطار مع الجاذبية والمنصات وتتبع الكاميرا (`MARIO_PLAYABLE_PASS_HOST`).
5. حلقات التحكم والتكرار ومكتبات GML البرمجية (`while`, `do...until`, `dsin`, `dcos`, `dtan`, `string_letters`, `string_lettersdigits`, `string_width`, `string_height`, `distance_to_point`, `ini_open/read/write`, `collision_circle`, `collision_line`, `collision_ellipse`).
6. فك أفعال DnD الشائعة وتطبيقها على الكائنات.
7. طابور تشغيل الصوت البرمجي وPitch/Pan والربط بـ JNI.
8. اجتياز 11 مجموعة اختبارات ذاتية ناتيف C بالكامل واختبار CMake libgm82_android بنجاح.

---

## ❌ المتبقي الكبير للوصول لتطابق ويندوز (REMAINING > 38%)

1. **Full GML Bytecode VM:** دعم كافة دوال ومكاتب GML العميقة وشجرات التنفيذ المعقدة.
2. **Precise Collision Masking:** اصطدام البكسل بدقة لكل سبرايت بدلاً من AABB/Shape bounding.
3. **GLES Hardware Pipeline:** رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد.
4. **OpenSL ES Audio Backend:** تشغيل الصوت الحقيقي المباشر على الجهاز.
5. **Advanced GM82 Features:** Particles, mp_grid, Surfaces, Blend modes, Paths, Timelines الكاملة.

---

## التعهد بالشفافية

عدم ادعاء "100%" أو "Complete Engine". النسبة الحالية هي **62%** حقيقية مع التوسع المستمر الصادق.
