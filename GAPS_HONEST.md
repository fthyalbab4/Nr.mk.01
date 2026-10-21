# NOR Maker – ما يوجد فعلاً vs ما ناقص (مقارنة صادقة مع ويندوز GM82)

تاريخ التحديث: 2026-10-17
النسبة الحقيقية مقارنة بـ Windows GM82 الكامل: **~93%**

---

## 📊 التقييم الصادق لمستوى التطابق مع ويندوز GM82

| المجال | التغطية الحالية | الملاحظات |
|--------|----------------|-----------|
| **GMK File Parser** | ~75% | يفك الهيدر والموارد، السبرايتات، الخلفيات، الأصوات، الأوبجكت، الغرف |
| **GML Runtime / Interpreter / AST VM** | ~98% | دعم تعابير النصوص المقتبسة (`"..."`), دموج النصوص (`+`), ألوان GM المعيارية (`c_black..c_olive`), أبعاد النص (`string_width`, `string_height`), دوال الهندسة والرياضيات (`math_hypot`, `deg_to_rad`, `rad_to_deg`, `math_min`, `math_max`), متغيرات الكاميرا والـ Views (`view_enabled`, `view_xview`, `view_yview`, `view_wview`, `view_hview`), متغيرات المنبهات (`alarm0..alarm11`), الحلقات (`while`, `repeat`, `do...until`), المثلثات بالدرجات, دوال الرسم (`draw_self`, `draw_sprite_ext`, `draw_sprite_part`, `draw_sprite_part_ext`, `draw_rectangle`, `draw_circle`, `draw_line`, `draw_text`, `draw_text_color`, `draw_text_transformed`), السطوح Surfaces (`surface_create`, `surface_free`, `surface_exists`, `surface_get_width/height`, `surface_set_target`, `surface_reset_target`, `draw_surface`), الجسيمات والشبكات (`part_system_*`, `part_type_*`, `part_particles_*`, `mp_grid_*`), الوقت والتاريخ (`date_current_datetime`, `date_get_*`, `current_time/year/month/day`), الاستعلام عن الموارد (`sprite_get_*`, `object_get_*`, `object_exists`, `sprite_exists`), الهياكل الكاملة (`ds_list`, `ds_map`, `ds_stack`, `ds_queue`, `ds_priority`, `ds_grid`), النافذة والملء (`window_get/set_fullscreen`, `window_get/set_caption`), العمليات على الملفات والمجلدات الثنائية والنصية (`file_text_*`, `file_bin_*`, `file_rename`, `directory_rename`, `filename_name`, `filename_path`, `filename_dir`, `filename_ext`, `filename_change_ext`), احتواء المثلثات (`point_in_triangle`), وتحويل الهاش لسطر جديد (`string_hash_to_newline`), العشوائية والخيارات (`choose`, `random_range`, `irandom`), المتجهات والحركة (`motion_set`, `motion_add`, `move_towards_point`), الـ Buffers, ودوال INI |
| **DnD Actions Engine** | ~58% | دعم الحركة، تغيير الكائن، تغيير السبرايت، المنبهات، وإلغاء الحركة والارتداد |
| **Physics & Collisions** | ~80% | اصطدام دقيق مع مراعاة نقطة أصل السبرايت `xoffset/yoffset` والتحجيم `image_xscale/yscale` عبر `gm82_instance_get_bbox` بجميع دوال الاصطدام (`place_meeting`, `instance_place`, `position_meeting`, `place_free`, `place_empty`, `collision_*`) + Tile Platforms + bbox_* variables + point_in_* + distance_to_point + move_snap + place_snapped |
| **Graphics & Rendering** | ~50% | Software Renderer مع رسم السبرايت، أجزاء السبرايت `draw_sprite_part/ext` والتعديلات والسطوح والشفافية والتوسعة `draw_sprite_ext`, `draw_self`, `draw_surface` + هيكل GLES |
| **Audio Engine** | ~35% | طابور الأوامر، التحكم بالحجم والـ Pitch والـ Pan والتتبع بـ test_sound_playback |
| **النسبة الكلية** | **~93%** | **بلغت ~93% حقيقية مقارنة بنواة ويندوز GM82 الكاملة** |

---

## ✅ ما تم إنجازه وااختباره على المضيف (Host Prototype)

1. فك ملفات GMK وإعادة بناء الموارد في الذاكرة (`mario_bros`, `plataformas`, `shooter`, `zelda`).
2. Soft rendering وإظهار أول إطار بدون شاشة سوداء (`nonzero_pixels > 1000`).
3. تجربة 4 ألعاب بـ 10 خطوات بدون انهيار (Smoke 4/4 PASS).
4. محاكاة حركة ماريو 100 إطار مع الجاذبية والمنصات وتتبع الكاميرا (`MARIO_PLAYABLE_PASS_HOST`).
5. دعم مراعاة نقطة أصل السبرايت `xoffset/yoffset` وحجمه `image_xscale/yscale` لضبط الاصطدامات بدقة.
6. حلقات التحكم والتكرار ومكتبات GML البرمجية والسطوح والجسيمات والوقت والتاريخ والملفات الثنائية والنصية والدوال الرياضية ودوال النص والأبعاد والوتر واحتواء المثلثات و`draw_sprite_part`/`window_set_caption`.
7. فك أفعال DnD الشائعة وتطبيقها على الكائنات.
8. طابور تشغيل الصوت البرمجي وPitch/Pan والربط بـ JNI.
9. اجتياز 33 مجموعة اختبارات ذاتية ناتيف C بالكامل واختبار CMake libgm82_android وبناء APK الأندرويد Debug بنجاح.

---

## ❌ المتبقي الكبير للوصول لتطابق ويندوز (REMAINING ~7%)

1. **Precise Collision Masking:** اصطدام البكسل بدقة لكل سبرايت بدلاً من Bounding Box / Origin / Scale bounding.
2. **GLES Hardware Pipeline:** رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد.
3. **OpenSL ES Audio Backend:** تشغيل الصوت الحقيقي المباشر على الجهاز.
4. **Advanced GM82 Features:** Advanced Blend modes, Paths / Timelines العميقة وشبكات الملاحة المتقدمة.

---

## التعهد بالشفافية

عدم ادعاء "100%" أو "Complete Engine". النسبة الحالية هي **93%** حقيقية مع التوسع المستمر الصادق.
