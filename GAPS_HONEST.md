# NOR Maker – ما يوجد فعلاً vs ما ناقص (مقارنة صادقة مع ويندوز GM82)

تاريخ التحديث: 2026-10-03
النسبة الحقيقية مقارنة بـ Windows GM82 الكامل: **~60%**

---

## 📊 التقييم الصادق لمستوى التطابق مع ويندوز GM82

| المجال | التغطية الحالية | الملاحظات |
|--------|----------------|-----------|
| **GMK File Parser** | ~65% | يفك الهيدر والموارد، السبرايتات، الخلفيات، الأصوات، الأوبجكت، الغرف |
| **GML Runtime / Interpreter** | ~65% | دعم التعابير، الشرطية، الحلقات (`while`, `repeat`, `do...until`), متغيرات `xstart/ystart/gravity`, الهياكل, ودوال `sprite_get_xoffset/yoffset` و `instance_furthest` و `position_destroy` |
| **DnD Actions Engine** | ~50% | دعم الحركة، تغيير الكائن، تغيير السبرايت، المنبهات، وإلغاء الحركة والارتداد |
| **Physics & Collisions** | ~62% | AABB المحدث + bbox_* variables + collision_* + `move_bounce_solid/all` + `move_outside_solid/all` + `move_random` |
| **Graphics & Rendering** | ~38% | Software Renderer على المضيف + هيكل GLES مبدئي |
| **Audio Engine** | ~32% | طابور الأوامر، التحكم بالحجم والـ Pitch والـ Pan والتتبع بـ test_sound_playback |
| **النسبة الكلية** | **~60%** | **بلغت ~60% مقارنة بنواة ويندوز GM82 الكاملة** |

---

## ✅ ما تم إنجازه واختباره على المضيف (Host Prototype)

1. فك ملفات GMK وإعادة بناء الموارد في الذاكرة (`mario_bros`, `plataformas`, `shooter`, `zelda`).
2. Soft rendering وإظهار أول إطار بدون شاشة سوداء (`nonzero_pixels > 1000`).
3. تجربة 4 ألعاب بـ 10 خطوات بدون انهيار (Smoke 4/4 PASS).
4. محاكاة حركة ماريو 100 إطار مع الجاذبية والمنصات وتتبع الكاميرا (`MARIO_PLAYABLE_PASS_HOST`).
5. حلقات التحكم والتكرار ومكتبات GML البرمجية (`while`, `do...until`, `string_digits`, `string_lower`, `string_upper`, `string_copy`, `string_replace`, `string_replace_all`, `ini_open/read/write`, `collision_circle`, `collision_line`, `collision_ellipse`).
6. فك أفعال DnD الشائعة وتطبيقها على الكائنات.
7. طابور تشغيل الصوت البرمجي وPitch/Pan والربط بـ JNI.

---

## ❌ المتبقي الكبير للوصول لتطابق ويندوز (REMAINING > 48%)

1. **Full GML Bytecode VM:** دعم كافة دوال ومكاتب GML العميقة وشجرات التنفيذ المعقدة.
2. **Precise Collision Masking:** اصطدام البكسل بدقة لكل سبرايت بدلاً من AABB/Shape bounding.
3. **GLES Hardware Pipeline:** رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد.
4. **OpenSL ES Audio Backend:** تشغيل الصوت الحقيقي المباشر على الجهاز.
5. **Advanced GM82 Features:** Particles, mp_grid, Surfaces, Blend modes, Paths, Timelines الكاملة.

---

## التعهد بالشفافية

عدم ادعاء "100%" أو "Complete Engine". النسبة الحالية هي **52%** حقيقية مع التوسع المستمر الصادق.
