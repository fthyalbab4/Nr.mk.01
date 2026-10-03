# تقدم مشروع NOR Maker / GM82

تاريخ التحديث: 2026-10-03
النسبة الفعلية مقارنة بـ Windows GM82 الكامل: **~62%**

## 📌 ملخص الوضع الحالي
- النواة تتطور كنسخة أولية على Host (تفك GMK، تمارس Soft Render، تحاكي فيزياء ماريو، تنفذ أفعال DnD وحلقات GML التحكمية `while`, `do...until`, ودوال المثلثات بالدرجات `dsin`, `dcos`, `dtan`, `darcsin`, `darccos`, `darctan`, `darctan2`, ودوال النصوص `string_letters`, `string_lettersdigits`, `string_width`, `string_height`, وحساب المسافات `distance_to_point`, وتتبع متغيرات الملاحة والفيزياء `xstart`, `ystart`, `xprevious`, `yprevious`, `gravity`, `friction` وحساب أحجام السبرايت المجمعة `image_xscale/yscale` باصطدامات الأشكال والحجم المتقدم).
- اجتياز 11 اختبار ذاتي ناتيف بالكامل وبناء المكتبة الناتيف `libgm82_android.so` عبر CMake بنجاح 100%.
- النسبة الإجمالية مقارنة بالمحرك الكامل لويندوز هي **62%**.

## 🗺️ خطة التطوير الشاملة للوصول لـ Core & GML Full Support

### المرحلة 1: مفسر وVM الـ GML (GML Bytecode VM Engine)
- دعم كامل لكافة تعابير ودوال GML العميقة وتمرير المعاملات المتقدمة ودوال المثلثات والنصوص بالدرجات.
- تحسين التعامل مع المصفوفات ثنائية الأبعاد والبُنى البياناتية المتعددة (`ds_list`, `ds_map`, `ds_stack`, `ds_queue`, `ds_priority`).

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
