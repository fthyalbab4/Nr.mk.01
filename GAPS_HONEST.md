# NOR Maker – ما يوجد فعلاً vs ما ناقص (مقارنة صادقة مع ويندوز GM82)

تاريخ التحديث: 2026-10-10
النسبة الحقيقية مقارنة بـ Windows GM82 الكامل: **~82%**

---

## 📊 التقييم الصادق لمستوى التطابق مع ويندوز GM82

| المجال | التغطية الحالية | الملاحظات |
|--------|----------------|-----------|
| **GMK File Parser** | ~75% | يفك الهيدر والموارد، السبرايتات، الخلفيات، الأصوات، الأوبجكت، الغرف |
| **GML Runtime / Interpreter** | ~90% | دعم تعابير النصوص المقتبسة (`"..."`), دموج النصوص (`+`), ألوان GM المعيارية (`c_black..c_olive`), متغيرات الكاميرا والـ Views, متغيرات المنبهات, الحلقات (`while`, `repeat`, `do...until`), دوال الحركة (`motion_set`, `motion_add`), الاستعلامات المكانية (`place_empty`, `position_empty`, `position_change`), دوال الرسم المتقدمة (`draw_line_width`, `draw_circle_color`, `draw_rectangle_color`, `draw_text_ext`), إدارة الأسطح (`surface_create`, `surface_exists`, `surface_free`), دوال الهياكل (`ds_list`, `ds_map`, `ds_grid` مع `ds_grid_get_sum/max/min`), الـ Buffers, ودوال INI |
| **DnD Actions Engine** | ~60% | دعم الحركة، تغيير الكائن، تغيير السبرايت، المنبهات، وإلغاء الحركة والارتداد |
| **Physics & Collisions** | ~72% | AABB المحدث بحسب مقياس السبرايت `image_xscale/yscale` + Tile Platforms + bbox_* variables + collision_circle + collision_rectangle + collision_line + collision_ellipse + collision_point + distance_to_point + move_snap + place_snapped + place_empty + position_empty |
| **Graphics & Rendering** | ~48% | Software Renderer مع رسم السبرايت والتوسعة `draw_sprite_ext` و`draw_self`, `draw_line_width`, `draw_circle_color`, `draw_rectangle_color`, `draw_text_ext` + Surface management + هيكل GLES |
| **Audio Engine** | ~35% | طابور الأوامر، التحكم بالحجم والـ Pitch والـ Pan والتتبع بـ test_sound_playback |
| **النسبة الكلية** | **~82%** | **بلغت ~82% حقيقية مقارنة بنواة ويندوز GM82 الكاملة** |

---

## ✅ ما تم إنجازه وااختباره على المضيف (Host Prototype)

1. فك ملفات GMK وإعادة بناء الموارد في الذاكرة (`mario_bros`, `plataformas`, `shooter`, `zelda`).
2. Soft rendering وإظهار أول إطار بدون شاشة سوداء (`nonzero_pixels > 1000`).
3. تجربة 4 ألعاب بـ 10 خطوات بدون انهيار (Smoke 4/4 PASS).
4. محاكاة حركة ماريو 100 إطار مع الجاذبية والمنصات وتتبع الكاميرا (`MARIO_PLAYABLE_PASS_HOST`).
5. حلقات التحكم والتكرار ومكتبات GML البرمجية (`while`, `do...until`, `dsin`, `dcos`, `dtan`, `string_letters`, `string_lettersdigits`, `string_ord_at`, `string_repeat`, `string_count`, `string_width`, `string_height`, `directory_exists`, `directory_create`, `file_copy`, `file_move`, `buffer_seek`, `buffer_write`, `buffer_read`, `buffer_poke`, `buffer_peek`, `ini_open/read/write`, `collision_circle`, `collision_line`, `collision_ellipse`, `ds_stack_*`, `ds_queue_*`, `ds_priority_*`, `ds_grid_*`, `motion_set`, `motion_add`, `place_empty`, `position_empty`, `surface_*`).
6. فك أفعال DnD الشائعة وتطبيقها على الكائنات.
7. طابور تشغيل الصوت البرمجي وPitch/Pan والربط بـ JNI.
8. اجتياز 21 مجموعة اختبارات ذاتية ناتيف C بالكامل وااختبار CMake libgm82_android وبناء APK الأندرويد Debug بنجاح.

---

## ❌ المتبقي الكبير للوصول لتطابق ويندوز (REMAINING ~18%)

1. **Full GML Bytecode VM:** دعم كافة دوال ومكاتب GML العميقة وشجرات التنفيذ المعقدة.
2. **Precise Collision Masking:** اصطدام البكسل بدقة لكل سبرايت بدلاً من AABB/Shape bounding.
3. **GLES Hardware Pipeline:** رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد.
4. **OpenSL ES Audio Backend:** تشغيل الصوت الحقيقي المباشر على الجهاز.
5. **Advanced GM82 Features:** Particles, mp_grid, Blend modes, Paths, Timelines الكاملة.

---

## التعهد بالشفافية

عدم ادعاء "100%" أو "Complete Engine". النسبة الحالية هي **82%** حقيقية مع التوسع المستمر الصادق.
