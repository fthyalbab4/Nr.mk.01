# تقدم مشروع NOR Maker / GM82

تاريخ التحديث: 2026-10-05
النسبة الفعلية مقارنة بـ Windows GM82 الكامل: **~72%**

## 📌 ملخص الوضع الحالي
- النواة تتطور كنسخة أولية على Host و Android (تفك GMK، تمارس Soft Render، تحاكي فيزياء ماريو، تنفذ أفعال DnD وحلقات GML التحكمية `while`, `do...until`, والتعابير النصية المقتبسة `"..."` والربط النصي `+` وثوابت الألوان المعيارية `c_black..c_olive` ومتغيرات الكاميرا `view_xview/yview/wview/hview/enabled` والمنبهات `alarm0..11`, ودوال المثلثات بالدرجات `dsin`, `dcos`, `dtan`, ودوال النصوص, وإدارة الملفات والمجلدات, وحساب المسافات, ومتغيرات الفيزياء وحزم البافرات `buffer_seek`, `buffer_write`, `buffer_read`).
- اجتياز 16 اختبار ذاتي ناتيف بالكامل وبناء المكتبة الناتيف `libgm82_android.so` عبر CMake وتجميع تطبيق الأندرويد `app-debug.apk` عبر Gradle بنجاح 100%.
- النسبة الإجمالية مقارنة بالمحرك الكامل لويندوز هي **72%**.

## 🗺️ خطة التطوير الشاملة للوصول لـ Core & GML Full Support

### المرحلة 1: مفسر وVM الـ GML (GML Bytecode VM Engine)
- دعم كامل لكافة تعابير ودوال GML العميقة وتمرير المعاملات المتقدمة ودوال المثلثات والنصوص بالدرجات ورمز المحارف `string_ord_at`.
- تحسين التعامل مع المصفوفات ثنائية الأبعاد والبُنى البياناتية المتعددة (`ds_list`, `ds_map`, `ds_stack`, `ds_queue`, `ds_priority`) والـ Buffers (`buffer_write`, `buffer_read`, `buffer_seek`).

### المرحلة 2: الاصطدامات الدقيقة (Precise Collision Masking)
- دعم الأشكال الصدامية `collision_line` و`collision_ellipse` و`collision_circle` و`collision_rectangle` و`collision_point` و`distance_to_point`.
- الانتقال المستقبلي إلى Per-Pixel Masking لكل سبرايت.

### المرحلة 3: معالجة العرض والجرافيكس العتادي (GLES Hardware Pipeline)
- رفع التكستشرات وإطارات الرسم للـ GPU على أندرويد عبر OpenGL ES.
- دعم الـ Surfaces والـ Blend Modes وشاشات الـ CRT Shaders.

### المرحلة 4: تشغيل الصوت العتادي (OpenSL ES Audio Backend)
- ربط طابور الأوامر الصوتية بمحرك OpenSL ES المباشر للأندرويد.

### المرحلة 5: الأنظمة المتقدمة (Advanced Engine Features)
- Particles Engine, mp_grid Pathfinding, Timelines, Paths.

## ❌ النواقص الأساسية للوصول لـ 100%
- مفسر GML bytecode كامل لجميع الدوال المعقدة.
- اصطدام البكسل الدقيق (Precise Masks).
- عرض الهاردوير عبر GLES وتكستشرات الـ GPU على أندرويد.
- التشغيل العتادي المباشر للصوت عبر OpenSL ES.
- الأنظمة المتقدمة (Particles, Data Structures, Surfaces, Networking).
