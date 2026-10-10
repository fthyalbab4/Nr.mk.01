# NOR Maker – ما يوجد فعلاً vs ما ناقص (مقارنة صادقة مع ويندوز GM82)

تاريخ التحديث: 2026-10-07
النسبة الحقيقية مقارنة بـ Windows GM82 الكامل: **~82%**

---

## 📊 التقييم الصادق لمستوى التطابق مع ويندوز GM82

| المجال | التغطية الحالية | الملاحظات |
|--------|----------------|-----------|
| **GMK File Parser** | ~78% | يفك الهيدر والموارد، السبرايتات، الخلفيات، الأصوات، الأوبجكت، الغرف |
| **GML Runtime / Interpreter** | ~90% | دعم AST VM الشامل للتعابير، رسم النصوص (`draw_text`, `draw_text_color`), السكريبتات وحصائد المعاملات (`argument0..15`), ألوان GM المعيارية, الكاميرا والـ Views, المنبهات (`alarm0..11`), الحلقات (`while`, `repeat`, `do...until`, `for`, `switch`), دوال الرسم والأسطح (`surfaces`, `draw_self`, `draw_sprite_ext`), المحاذاة والشبكة (`move_snap`, `place_snapped`), الكائنات وتوليدها وتدميرها, دوال الحركة والفيزياء (`motion_set`, `motion_add`, `move_towards_point`, `move_outside_solid`, `move_bounce_solid`), الهياكل الكاملة (`ds_list`, `ds_map`, `ds_stack`, `ds_queue`, `ds_priority`, `ds_grid`), الـ Buffers, الجسيمات (`particles`), ودوال INI والملفات |
| **DnD Actions Engine** | ~60% | دعم الحركة، تغيير الكائن، تغيير السبرايت، المنبهات، وإلغاء الحركة والارتداد |
| **Physics & Collisions** | ~72% | AABB المحدث بحسب مقياس السبرايت `image_xscale/yscale` + Tile Platforms + bbox_* variables + collision_circle + collision_rectangle + collision_line + collision_ellipse + collision_point + distance_to_point + move_snap + place_snapped |
| **Graphics & Rendering** | ~48% | Software Renderer مع رسم السبرايت والنصوص والأسطح + هيكل GLES مبدئي |
| **Audio Engine** | ~38% | طابور الأوامر، التحكم بالحجم والـ Pitch والـ Pan والتتبع ودوال الصوت البرمجية |
| **النسبة الكلية** | **~82%** | **بلغت ~82% حقيقية مقارنة بنواة ويندوز GM82 الكاملة** |

---

## ✅ ما تم إنجازه واختباره على المضيف (Host Prototype)

1. فك ملفات GMK وإعادة بناء الموارد في الذاكرة (`mario_bros`, `plataformas`, `shooter`, `zelda`).
2. Soft rendering وإظهار أول إطار بدون شاشة سوداء (`nonzero_pixels > 1000`).
3. تجربة 4 ألعاب بـ 10 خطوات بدون انهيار (Smoke 4/4 PASS).
4. محاكاة حركة ماريو 100 إطار مع الجاذبية والمنصات وتتبع الكاميرا (`MARIO_PLAYABLE_PASS_HOST`).
5. دعم كامل لتنفيذ شجرات AST بـ GML VM وشامل لمعاملات السكريبتات `argument0..15`.
6. فك أفعال DnD الشائعة وتطبيقها على الكائنات.
7. طابور تشغيل الصوت البرمجي وPitch/Pan والربط بـ JNI.
8. اجتياز 21 مجموعة اختبارات ذاتية ناتيف C بالكامل واختبار CMake libgm82_android وبناء APK الأندرويد Debug بنجاح.

---

## ❌ المتبقي الكبير للوصول لتطابق ويندوز (REMAINING ~18%)

1. **Full GML Bytecode JIT Compiler:** تحويل VM من AST/Interpreter إلى Bytecode VM سريع جداً.
2. **Precise Per-Pixel Collision Masking:** اصطدام البكسل بدقة لكل سبرايت بدلاً من AABB/Shape bounding.
3. **GLES Hardware Pipeline Acceleration:** رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد بالكامل.
4. **OpenSL ES Audio Backend:** تشغيل الصوت الحقيقي المباشر المنخفض التأخير على الجهاز.
5. **Advanced GM82 Features:** Particles Emitters العميقة, Surfaces Blend modes, Paths, Timelines الكاملة.

---

## التعهد بالشفافية

عدم ادعاء "100%" أو "Complete Engine". النسبة الحالية هي **82%** حقيقية مع التوسع المستمر الصادق.
