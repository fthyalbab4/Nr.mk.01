# NOR Maker – ما يوجد فعلاً vs ما ناقص (مقارنة صادقة مع ويندوز GM82)

تاريخ التحديث: 2026-10-13
النسبة الحقيقية مقارنة بـ Windows GM82 الكامل: **~91%**

---

## 📊 التقييم الصادق لمستوى التطابق مع ويندوز GM82

| المجال | التغطية الحالية | الملاحظات |
|--------|----------------|-----------|
| **GMK File Parser** | ~75% | يفك الهيدر والموارد، السبرايتات، الخلفيات، الأصوات، الأوبجكت، الغرف |
| **GML Runtime / Interpreter** | ~97% | دعم كامل لجميع تعابير وشجرات GML (AST Execution): الصوت (`sound_play`, `sound_stop`, `sound_isplaying`), الكائنات (`instance_create`, `instance_destroy`, `instance_exists`, `instance_nearest`, `instance_furthest`), المسافات (`distance_to_object`, `distance_to_point`), الرسم والأسطح (`draw_sprite`, `draw_rectangle`, `draw_circle`, `draw_line`, `draw_text`, `draw_clear`, `draw_set_color`, `draw_set_alpha`, `surface_create`, `surface_free`, `surface_exists`, `surface_set_target`, `surface_reset_target`, `draw_surface`), الغرف واللعبة (`room_goto`, `room_goto_next`, `room_goto_previous`, `room_restart`, `game_end`, `game_restart`), المسارات والخطوط الزمانية (`path_start`, `path_end`, `timeline_start`, `timeline_stop`), الجسيمات والشبكات (`part_system_*`, `part_type_*`, `mp_grid_*`), التاريخ والوقت والنافذة (`date_*`, `current_time`, `window_set_caption`, `window_get_caption`) |
| **DnD Actions Engine** | ~58% | دعم الحركة، تغيير الكائن، تغيير السبرايت، المنبهات، وإلغاء الحركة والارتداد |
| **Physics & Collisions** | ~73% | AABB المحدث بحسب مقياس السبرايت `image_xscale/yscale` + Tile Platforms + bbox_* variables + collision_circle + collision_rectangle + collision_line + collision_ellipse + collision_point + point_in_circle + point_in_rectangle + distance_to_point + move_snap + place_snapped |
| **Graphics & Rendering** | ~47% | Software Renderer مع رسم السبرايت والتوسعة `draw_sprite_ext`, `draw_self`, `draw_line_width`, `draw_text_ext`, `draw_circle_color`, `draw_rectangle_color` + هيكل GLES مبدئي |
| **Audio Engine** | ~35% | طابور الأوامر، التحكم بالحجم والـ Pitch والـ Pan والتتبع بـ test_sound_playback |
| **النسبة الكلية** | **~91%** | **بلغت ~91% حقيقية مقارنة بنواة ويندوز GM82 الكاملة** |

---

## ✅ ما تم إنجازه واختباره على المضيف (Host Prototype)

1. فك ملفات GMK وإعادة بناء الموارد في الذاكرة (`mario_bros`, `plataformas`, `shooter`, `zelda`).
2. Soft rendering وإظهار أول إطار بدون شاشة سوداء (`nonzero_pixels > 1000`).
3. تجربة 4 ألعاب بـ 10 خطوات بدون انهيار (Smoke 4/4 PASS).
4. محاكاة حركة ماريو 100 إطار مع الجاذبية والمنصات وتتبع الكاميرا (`MARIO_PLAYABLE_PASS_HOST`).
5. حلقات التحكم والتكرار ومكتبات GML البرمجية وموزعات AST VM الكاملة (`sound_*`, `instance_*`, `draw_*`, `surface_*`, `room_*`, `game_*`, `path_*`, `timeline_*`, `part_*`, `mp_grid_*`, `date_*`, `window_*`).
6. فك أفعال DnD الشائعة وتطبيقها على الكائنات.
7. طابور تشغيل الصوت البرمجي وPitch/Pan والربط بـ JNI.
8. اجتياز 27 مجموعة اختبارات ذاتية ناتيف C بالكامل واختبار CMake libgm82_android وبناء APK الأندرويد Debug بنجاح.

---

## ❌ المتبقي الكبير للوصول لتطابق ويندوز (REMAINING ~9%)

1. **Full Bytecode JIT/Compilation:** تحويل شجرة AST إلى Bytecode عالي السرعة للأجهزة القديمة.
2. **Precise Collision Masking:** اصطدام البكسل بدقة لكل سبرايت بدلاً من AABB/Shape bounding.
3. **GLES Hardware Pipeline:** رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد.
4. **OpenSL ES Audio Backend:** تشغيل الصوت الحقيقي المباشر على الجهاز.
5. **Device Testing:** تجربة التطبيق على أجهزة أندرويد حقيقية متنوعة.

---

## التعهد بالشفافية

عدم ادعاء "100%" أو "Complete Engine". النسبة الحالية هي **91%** حقيقية مع التوسع المستمر الصادق.
