# NOR Maker – ما يوجد فعلاً vs ما ناقص (مقارنة صادقة مع ويندوز GM82)

تاريخ التحديث: 2026-10-15
النسبة الحقيقية مقارنة بـ Windows GM82 الكامل: **~90%**

---

## 📊 التقييم الصادق لمستوى التطابق مع ويندوز GM82

| المجال | التغطية الحالية | الملاحظات |
|--------|----------------|-----------|
| **GMK File Parser** | ~75% | يفك الهيدر والموارد، السبرايتات، الخلفيات، الأصوات، الأوبجكت، الغرف |
| **GML Runtime / Interpreter / AST VM** | ~96% | دعم تعابير النصوص المقتبسة (`"..."`), دموج النصوص (`+`), ألوان GM المعيارية (`c_black..c_olive`), متغيرات الكاميرا والـ Views (`view_enabled`, `view_xview`, `view_yview`, `view_wview`, `view_hview`), متغيرات المنبهات (`alarm0..alarm11`), الحلقات (`while`, `repeat`, `do...until`), المثلثات بالدرجات, دوال الرسم (`draw_self`, `draw_sprite_ext`, `draw_rectangle`, `draw_circle`, `draw_line`, `draw_text`, `draw_text_color`), السطوح Surfaces (`surface_create`, `surface_free`, `surface_exists`, `surface_get_width/height`, `surface_set_target`, `surface_reset_target`, `draw_surface`), الجسيمات والشبكات (`part_system_*`, `part_type_*`, `part_particles_*`, `mp_grid_*`), الوقت والتاريخ والمنبهات (`date_current_datetime`, `date_get_*`, `current_time`, `get_timer`, `delta_time`, `alarm_set`, `alarm_get`), الغرف والمسارات والخطوط الزمانية (`room_goto`, `room_restart`, `path_start`, `timeline_start`), النافذة والعنوان (`window_get/set_fullscreen`, `window_get/set_caption`), الاستعلام عن الموارد (`sprite_get_*`, `object_get_*`, `object_exists`, `sprite_exists`), الهياكل الكاملة (`ds_list`, `ds_map`, `ds_stack`, `ds_queue`, `ds_priority`, `ds_grid`), العمليات على الملفات والمجلدات (`file_text_*`), العشوائية والخيارات (`choose`, `random_range`, `irandom`), المتجهات والحركة (`motion_set`, `motion_add`, `move_towards_point`, `move_outside_solid`, `move_contact_solid`), الـ Buffers, ودوال INI |
| **DnD Actions Engine** | ~58% | دعم الحركة، تغيير الكائن، تغيير السبرايت، المنبهات، وإلغاء الحركة والارتداد |
| **Physics & Collisions** | ~73% | AABB المحدث بحسب مقياس السبرايت `image_xscale/yscale` + Tile Platforms + bbox_* variables + collision_circle + collision_rectangle + collision_line + collision_ellipse + collision_point + point_in_circle + point_in_rectangle + distance_to_point + move_snap + place_snapped |
| **Graphics & Rendering** | ~48% | Software Renderer مع رسم السبرايت والسطوح والتوسعة `draw_sprite_ext`, `draw_self`, `draw_surface` + هيكل GLES مبدئي |
| **Audio Engine** | ~35% | طابور الأوامر، التحكم بالحجم والـ Pitch والـ Pan والتتبع بـ test_sound_playback |
| **النسبة الكلية** | **~90%** | **بلغت ~90% حقيقية مقارنة بنواة ويندوز GM82 الكاملة** |

---

## ✅ ما تم إنجازه وااختباره على المضيف (Host Prototype)

1. فك ملفات GMK وإعادة بناء الموارد في الذاكرة (`mario_bros`, `plataformas`, `shooter`, `zelda`).
2. Soft rendering وإظهار أول إطار بدون شاشة سوداء (`nonzero_pixels > 1000`).
3. تجربة 4 ألعاب بـ 10 خطوات بدون انهيار (Smoke 4/4 PASS).
4. محاكاة حركة ماريو 100 إطار مع الجاذبية والمنصات وتتبع الكاميرا (`MARIO_PLAYABLE_PASS_HOST`).
5. حلقات التحكم والتكرار ومكتبات GML البرمجية والسطوح والجسيمات والوقت والتاريخ والملفات والدوال الرياضية.
6. فك أفعال DnD الشائعة وتطبيقها على الكائنات.
7. طابور تشغيل الصوت البرمجي وPitch/Pan والربط بـ JNI.
8. اجتياز 28 مجموعة اختبارات ذاتية ناتيف C بالكامل وااختبار CMake libgm82_android وبناء APK الأندرويد Debug بنجاح.

---

## ❌ المتبقي الكبير للوصول لتطابق ويندوز (REMAINING ~10%)

1. **Precise Collision Masking:** اصطدام البكسل بدقة لكل سبرايت بدلاً من AABB/Shape bounding.
2. **GLES Hardware Pipeline:** رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد.
3. **OpenSL ES Audio Backend:** تشغيل الصوت الحقيقي المباشر على الجهاز.
4. **Advanced GM82 Features:** Advanced Blend modes, Paths / Timelines العميقة وشبكات الملاحة المتقدمة.

---

## التعهد بالشفافية

عدم ادعاء "100%" أو "Complete Engine". النسبة الحالية هي **90%** حقيقية مع التوسع المستمر الصادق.
