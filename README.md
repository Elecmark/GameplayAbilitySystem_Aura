# Unreal Engine 5 - Gameplay Ability System - Top Down RPG / 虚幻引擎5 - 游戏技能系统 - 俯视角RPG

Create a multiplayer RPG with Unreal Engine's Gameplay Ability System (GAS)! / 使用虚幻引擎的游戏技能系统（GAS）创建一款多人RPG！

## 0.Introduction / 介绍

- 1个讲座·12 分钟 / 1 Lecture · 12 Minutes

- Introduction

  12:07
  



## 1.Project Creation / 项目创建

- 14 个讲座·2 小时 26 分钟 / 14 Lectures · 2 Hours 26 Minutes

- Jetbrains Rider - Now Free for Non-Commercial Use!

  00:26

- 

  Project Creation

  04:56

- 

  Setting up Version Control (Optional)

  07:26

- 

  The Base Character Class

  10:00
  
### 🧙‍♀️ **AuraCharacterBase 基类分析**

  #### **类定义特性**
  ```cpp
  UCLASS(Abstract)  // 标记为抽象类，不能被实例化
  ```
  - **抽象基类**：只作为父类被继承，不能直接放置到关卡中
  - **设计目的**：提供通用功能给英雄和敌人角色共享

  #### **构造函数核心实现**

  ##### **1. Tick 系统禁用**
  ```cpp
  PrimaryActorTick.bCanEverTick = false;
  ```
  - **性能优化**：角色不需要每帧更新时禁用 Tick
  - **典型场景**：AI 控制或简单动画的角色

  ##### **2. 武器组件创建**
  ```cpp
  Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
  ```
  - **`CreateDefaultSubobject`**：在构造函数中创建组件
  - **命名规范**：给组件指定唯一名称便于调试

  ##### **3. 武器附着系统**
  ```cpp
  Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
  ```
  - **附着到骨骼**：绑定到网格体的指定骨骼插槽
  - **`FName("WeaponHandSocket")`**：使用骨骼插槽名称
  - **动画同步**：武器随角色动画自然移动

  ##### **4. 碰撞设置**
  ```cpp
  Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  ```
  - **`NoCollision`**：禁用武器所有碰撞
  - **设计考虑**：
    - 避免武器与环境的意外碰撞
    - 伤害检测通过其他系统实现（如伤害盒）

  #### **UPROPERTY 配置**
  ```cpp
  UPROPERTY(EditAnywhere, Category = "Combat")
  TObjectPtr<USkeletalMeshComponent> Weapon;
  ```
  - **`EditAnywhere`**：可在蓝图实例和类默认值中编辑
  - **`Category = "Combat"`**：在编辑器"Combat"分类下显示
  - **`TObjectPtr`**：UE5 安全指针，自动处理垃圾回收

  #### **架构设计意义**

  ##### **继承结构**
  ```
  ACharacter (UE基类)
      ↓
  AAuraCharacterBase (项目基类)
      ├── AHeroCharacter (玩家英雄)
      └── AEnemyCharacter (敌人)
  ```

  ##### **职责分离**
  1. **基类**：通用组件（武器、基础属性）
  2. **派生类**：特定逻辑（玩家控制、AI行为）

  #### **扩展建议**

  ##### **武器系统增强**
  ```cpp
  // 可添加的扩展功能
  virtual void EquipWeapon(USkeletalMesh* NewWeaponMesh);
  virtual void Attack();
  virtual void TakeDamage(float DamageAmount);
  ```

  ##### **GAS 集成预留**
  ```cpp
  // 为后续游戏技能系统准备
  class UAbilitySystemComponent;
  class UAttributeSet;
  ```

  #### **最佳实践**
  - **抽象基类**：提取公共功能，减少代码重复

  - **组件化设计**：武器作为独立组件便于更换

  - **性能优化**：无Tick需求时及时禁用

  - **碰撞分离**：攻击检测与渲染碰撞分开处理

- 

  Player and Enemy Characters

  06:09

- 

  Character Blueprint Setup

  12:13

- 

  Animation Blueprints

  14:30

- 

  Enhanced Input

  09:43

- 

  Aura Player Controller

  11:14
  
### 🎮 AuraPlayerController 分析
  - 自定义玩家控制器，管理输入系统和光标控制
  - 使用 UE5 **增强输入系统（Enhanced Input）**
  - 支持 **多人游戏网络复制**

  #### 🔑 **核心特性**

  ##### **1. 构造函数设置**
  ```cpp
  bReplicates = true;  // 启用网络复制
  ```
  - **作用**：让服务器同步控制器状态到所有客户端

  ##### **2. 输入系统初始化**
  ```cpp
  Subsystem->AddMappingContext(AuraContext, 0);
  ```
  - **优先级 0**：基础输入上下文
  - 使用 `EditAnywhere` 可在编辑器中灵活设置

  ##### **3. 光标控制配置**
  ```cpp
  bShowMouseCursor = true;
  DefaultMouseCursor = EMouseCursor::Default;
  ```
  - 显示鼠标光标
  - 使用默认光标样式

  ##### **4. 输入模式设置**
  ```cpp
  FInputModeGameAndUI InputModeData;
  ```
  - **游戏+UI混合模式**：同时处理游戏输入和UI交互
  - **不锁定鼠标**：可自由移动出视口
  - **输入时不隐藏光标**：保持可见性

  #### ⚙️ **技术要点**
  - **`check()`**：开发时断言验证资源有效性
  - **`TObjectPtr`**：UE5 推荐的对象指针（自动垃圾回收）
  - **`UEnhancedInputLocalPlayerSubsystem`**：管理输入上下文的核心系统

  #### 🎯 **应用场景**
  - 俯视角 RPG 游戏
  - 需要鼠标交互的游戏
  - 多人网络游戏

- 

  Movement Input

  16:14

### 🔄 **AuraPlayerController 输入系统升级**

  #### **新增输入动作支持**
  ```cpp
  UPROPERTY(EditAnywhere, Category = "Input")
  TObjectPtr<UInputAction> MoveAction;  // 移动输入动作资源
  ```
  - **`UInputAction`**：增强输入系统的原子输入单位
  - **可配置性**：在编辑器中指定具体的移动输入动作

  #### **输入组件重写**
  ```cpp
  virtual void SetupInputComponent() override;  // 重写输入绑定
  ```
  - **扩展点**：在父类基础上添加增强输入绑定
  - **最佳实践**：将输入绑定逻辑集中在此函数

  #### **SetupInputComponent 实现**

  ##### **1. 获取增强输入组件**
  ```cpp
  UEnhancedInputComponent* EnhancedInputComponent = 
      CastChecked<UEnhancedInputComponent>(InputComponent);
  ```
  - **`CastChecked`**：安全转换，失败时断言崩溃
  - **类型转换**：将基础输入组件转为增强输入组件

  ##### **2. 输入动作绑定**
  ```cpp
  EnhancedInputComponent->BindAction(
      MoveAction,                    // 输入动作资源
      ETriggerEvent::Triggered,      // 触发事件类型
      this,                         // 绑定的对象
      &AAuraPlayerController::Move   // 回调函数
  );
  ```
  - **`ETriggerEvent::Triggered`**：输入持续触发（如按住按键）
  - **函数指针绑定**：将输入事件连接到成员函数

  #### **Move 函数实现**

  ##### **1. 输入值解析**
  ```cpp
  const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
  ```
  - **`FVector2D`**：二维输入向量（WASD/摇杆）
  - **泛型获取**：安全获取指定类型的输入值

  ##### **2. 方向计算**
  ```cpp
  const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);  // 仅保留偏航角
  ```
  - **简化旋转**：俯视角游戏中忽略俯仰和滚转
  - **偏航角**：控制角色左右旋转的方向

  ##### **3. 坐标系转换**
  ```cpp
  const FVector ForwardDirection = 
      FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);  // 前方向
  const FVector RightDirection = 
      FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);   // 右方向
  ```
  - **旋转矩阵**：将局部方向转换为世界方向
  - **坐标系**：
    - **X轴**：前方向（前进/后退）
    - **Y轴**：右方向（左移/右移）

  ##### **4. 应用移动输入**
  ```cpp
  ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
  ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
  ```
  - **向量分解**：
    - **Y分量**：控制前后移动（W/S）
    - **X分量**：控制左右移动（A/D）
  - **叠加效果**：同时按下两个方向键产生对角线移动

  #### **输入流程总结**
  ```
  输入动作 → 绑定到函数 → 解析向量 → 计算方向 → 应用移动
  ```

  #### **设计优势**

  ##### **1. 灵活性**
  - 输入映射可在编辑器中配置
  - 支持多种输入设备（键盘、手柄、鼠标）

  ##### **2. 模块化**
  - 输入动作与逻辑分离
  - 便于扩展新输入功能

  ##### **3. 维护性**
  - 所有输入绑定集中管理
  - 清晰的输入处理流程

- 

  Game Mode

  13:23

### 🤺 **AuraCharacter 玩家角色配置**

  #### **角色运动组件设置**

  ##### **1. 朝向运动方向**
  ```cpp
  GetCharacterMovement()->bOrientRotationToMovement = true;
  ```
  - **自动朝向**：角色自动转向移动方向
  - **俯视角特性**：视觉上更自然的移动反馈
  - **替代方案**：如果设为 `false`，需要手动控制旋转

  ##### **2. 旋转速度配置**
  ```cpp
  GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
  ```
  - **`FRotator(Pitch, Yaw, Roll)`**：三维旋转速度
  - **Yaw 400**：适中的偏航旋转速度（每秒度数）
  - **Pitch/Roll 为 0**：俯视角游戏通常不需要俯仰和滚转

  ##### **3. 平面约束**
  ```cpp
  GetCharacterMovement()->bConstrainToPlane = true;
  GetCharacterMovement()->bSnapToPlaneAtStart = true;
  ```
  - **平面锁定**：将角色运动限制在水平面（X-Y平面）
  - **初始对齐**：游戏开始时自动对齐到平面
  - **俯视角必要性**：防止意外的Z轴移动

  #### **控制器旋转配置**

  ##### **禁用控制器旋转继承**
  ```cpp
  bUseControllerRotationPitch = false;
  bUseControllerRotationYaw = false;
  bUseControllerRotationRoll = false;
  ```
  - **完全解耦**：角色旋转与控制器旋转完全独立
  - **设计目的**：
    - 允许控制器单独控制相机
    - 角色根据移动方向自动旋转
    - 俯视角游戏的典型配置

  #### **配置效果总结**

  ##### **运动行为**
  ```
  输入处理 → 方向计算 → 自动旋转 → 应用移动
  ```

  ##### **旋转关系**
  ```
  控制器（控制相机）
      ↓ 独立
  角色（根据运动方向自动旋转）
  ```

  #### **对比分析**

  | 设置                        | 开启效果         | 关闭效果     |
  | --------------------------- | ---------------- | ------------ |
  | `bOrientRotationToMovement` | 自动面向移动方向 | 保持当前方向 |
  | `bUseControllerRotationYaw` | 跟随控制器偏转   | 旋转独立     |
  | `bConstrainToPlane`         | 锁定水平移动     | 允许垂直移动 |

  #### **扩展应用**

  ##### **动画适配**
  - 自动朝向确保动画播放方向正确
  - 移动停止时保持最后朝向

  ##### **网络同步**
  - 旋转变化会通过网络复制
  - 确保所有客户端看到一致的角色朝向

  ##### **后续扩展点**
  ```cpp
  // 可添加的额外运动配置
  GetCharacterMovement()->MaxWalkSpeed = 600.f;    // 最大速度
  GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;  // 刹车减速度
  GetCharacterMovement()->GroundFriction = 8.f;     // 地面摩擦力
  ```

  #### **最佳实践建议**
  1. **俯视角游戏**：保持这些设置不变
  2. **第一/第三人称**：需要调整 `bUseControllerRotationYaw = true`
  3. **特殊移动**：可能需要禁用平面约束
  4. **性能考量**：合理的旋转速率避免视觉不适

- 

  Enemy Interface

  06:38

### 🎯 **Unreal Engine 接口系统深入解析**

  #### **双类接口架构**

  ##### **1. UInterface（反射类）**
  ```cpp
  UINTERFACE(MinimalAPI)
  class UEnemyInterface : public UInterface
  {
      GENERATED_BODY()
  };
  ```
  - **`UINTERFACE`**：UE宏，声明反射接口类
  - **`MinimalAPI`**：仅导出必要函数，减少编译依赖
  - **继承自 `UInterface`**：UE接口基类
  - **作用**：为反射系统提供类型信息，可在蓝图中使用

  ##### **2. IInterface（功能类）**
  ```cpp
  class GAS_AURA_API IEnemyInterface
  {
      GENERATED_BODY()
  public:
      virtual void HighlightActor() = 0;    // 纯虚函数
      virtual void UnHighlightActor() = 0;  // 纯虚函数
  };
  ```
  - **功能承载**：实际定义接口方法
  - **`GENERATED_BODY`**：允许使用UE反射功能
  - **核心**：纯虚函数声明，强制派生类实现

---

  #### ⚡ **纯虚函数详解**

  ##### **语法特征**
  ```cpp
  virtual void FunctionName() = 0;
  ```
  - **`= 0`**：纯虚函数标记
  - **抽象性**：没有默认实现
  - **强制性**：派生类必须实现

  ##### **纯虚函数 vs 普通虚函数**

  | 特性     | 纯虚函数             | 普通虚函数       |
  | -------- | -------------------- | ---------------- |
  | 定义     | `= 0`                | 有默认实现       |
  | 实现要求 | **必须**在派生类实现 | 可选覆盖         |
  | 类类型   | 使类成为**抽象类**   | 不强制           |
  | 实例化   | **不能**直接实例化   | 可以直接实例化   |
  | UE中用途 | 接口定义             | 基类提供默认行为 |

---

  #### 🏗️ **接口设计模式**

  ##### **1. 角色高亮接口**
  ```cpp
  // 敌人交互可视化
  virtual void HighlightActor() = 0;    // 鼠标悬停时高亮
  virtual void UnHighlightActor() = 0;  // 鼠标离开时取消高亮
  ```

  ##### **2. 实现示例**
  ```cpp
  // 在敌人角色类中实现
  class AEnemyCharacter : public ACharacter, public IEnemyInterface
  {
  public:
      virtual void HighlightActor() override
      {
          // 实现高亮逻辑：改变材质、添加轮廓等
      }
      
      virtual void UnHighlightActor() override
      {
          // 恢复默认状态
      }
  };
  ```

---

  #### 🔧 **UE接口系统工作机制**

  ##### **反射系统集成**
  ```cpp
  // 接口检查
  if (Actor->Implements<UEnemyInterface>())
  {
      IEnemyInterface* Enemy = Cast<IEnemyInterface>(Actor);
      if (Enemy)
      {
          Enemy->HighlightActor();
      }
  }
  ```

  ##### **蓝图支持**
  - 可在蓝图中实现接口
  - 编辑器可识别接口类型
  - 支持接口事件调度

---

  #### 🎮 **游戏中的应用场景**

  ##### **敌人交互系统**
  ```mermaid
  flowchart TD
      Player[玩家鼠标悬停] --> Check[检查接口]
      Check -->|有接口| Highlight[调用高亮]
      Check -->|无接口| Ignore[忽略]
      PlayerLeave[鼠标离开] --> Unhighlight[取消高亮]
  ```

  ##### **多态调用**
  ```cpp
  // 统一处理不同类型的敌人
  TArray<AActor*> Enemies;
  for (AActor* Enemy : Enemies)
  {
      if (Enemy->Implements<UEnemyInterface>())
      {
          IEnemyInterface::Execute_HighlightActor(Enemy);
      }
  }
  ```

---

  #### 💡 **设计优势**

  ##### **1. 松耦合设计**
  - 调用者不关心具体实现类
  - 只需知道接口协议

  ##### **2. 扩展性**
  ```cpp
  // 可轻松添加新接口
  class IInteractable
  {
  public:
      virtual void Interact() = 0;
  };
  
  // 类实现多个接口
  class AEnemy : public IEnemyInterface, public IInteractable
  {
      // 实现两个接口的方法...
  };
  ```

  ##### **3. 类型安全**
  - 编译时检查接口实现
  - 避免运行时错误

---

  #### 🚀 **最佳实践**

  ##### **接口命名规范**
  - **`I` 前缀**：功能接口类（`IEnemyInterface`）
  - **`U` 前缀**：反射接口类（`UEnemyInterface`）
  - **清晰语义**：接口名描述能力而非身份

  ##### **纯虚函数设计**
  ```cpp
  // 好的设计：职责单一
  virtual void TakeDamage(float Damage) = 0;
  
  // 避免：功能过于复杂
  virtual void HandleCombat(float Damage, FVector Location, AActor* Instigator) = 0;
  ```

  ##### **UE特定技巧**
  ```cpp
  // 蓝图可调用版本
  UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
  void HighlightActor();
  virtual void HighlightActor_Implementation();  // C++实现
  
  // 纯C++版本
  virtual void HighlightActor() = 0;  // 强制实现
  ```

---

  #### 📊 **总结对比**

  | 方案         | 接口         | 继承       | 组件       |
  | ------------ | ------------ | ---------- | ---------- |
  | **适用场景** | 跨类共享能力 | 类层次关系 | 功能模块化 |
  | **耦合度**   | **低**       | 高         | 中         |
  | **灵活性**   | **高**       | 低         | **高**     |
  | **UE集成**   | 蓝图友好     | 标准继承   | 编辑器友好 |
  | **性能**     | 虚函数开销   | 虚函数开销 | 可能更高   |

  **接口最适合**：定义"能做什么"而不是"是什么"，特别是需要跨不同类层次共享功能的场景。

- 

  Highlight Enemies

  19:25

### 🎯 **鼠标光标追踪与敌人高亮系统**

  ##### **新增核心成员变量**
  ```cpp
  void CursorTrace();  // 光标追踪方法
  TObjectPtr<IEnemyInterface> LastActor;  // 上一帧的敌人接口
  TObjectPtr<IEnemyInterface> ThisActor;  // 当前帧的敌人接口
  ```

---

  #### 🔍 **PlayerTick 集成**
  ```cpp
  virtual void PlayerTick(float DeltaTime) override;
  ```
  - **每帧调用**：启用 Actor Tick 系统
  - **实时检测**：持续追踪鼠标下方的敌人
  - **性能考虑**：简单的射线检测开销较小

---

  #### 🎮 **CursorTrace 方法详解**

  ##### **1. 光线投射检测**
  ```cpp
  FHitResult CursorHit;
  GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
  ```
  - **`GetHitResultUnderCursor`**：从光标位置发射射线
  - **`ECC_Visibility`**：可见性碰撞通道
  - **`false`**：不启用复杂碰撞检测
  - **返回值**：包含击中信息的 `FHitResult`

  ##### **2. 接口指针转换**
  ```cpp
  LastActor = ThisActor;  // 保存上一帧结果
  ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());
  ```
  - **`Cast<IEnemyInterface>`**：尝试将 Actor 转换为敌人接口
  - **自动转换**：如果 Actor 实现了接口，返回有效指针；否则返回 `nullptr`
  - **指针传递**：使用 `TObjectPtr` 安全存储接口指针

---

  #### 🧠 **智能状态管理逻辑**

  ##### **状态转移矩阵**
  ```
  上一帧 (LastActor)  当前帧 (ThisActor)  操作
      null                null         无操作 (A)
      null                有效         高亮当前 (B)
      有效                null         取消高亮上一帧 (C)
      有效                有效(不同)   取消高亮上一帧，高亮当前 (D)
      有效                有效(相同)   无操作 (E)
  ```

  ##### **代码实现逻辑**
  ```cpp
  // 情况 C 和 D：需要取消上一帧的高亮
  if (LastActor != nullptr && LastActor != ThisActor)
  {
      LastActor->UnHighlightActor();  // 调用接口方法
  }
  
  // 情况 B 和 D：需要高亮当前帧
  if (ThisActor != nullptr && ThisActor != LastActor)
  {
      ThisActor->HighlightActor();  // 调用接口方法
  }
  ```

---

  #### 💡 **设计亮点分析**

  ##### **1. 无状态切换优化**
  ```cpp
  // 避免了重复调用
  if (ThisActor == LastActor) return;  // 显式优化
  // 但代码中通过条件判断隐式实现
  ```

  ##### **2. 接口安全调用**
  ```cpp
  // 空指针安全检查已包含在条件判断中
  LastActor->UnHighlightActor();  // 仅在 LastActor 非空时调用
  ```

  ##### **3. 帧间连贯性**
  - **平滑过渡**：从高亮到非高亮状态自然切换
  - **无闪烁**：避免同一物体反复高亮/取消

---

  #### 🚀 **扩展可能性**

  ##### **性能优化**
  ```cpp
  // 可添加距离检测
  float Distance = FVector::Distance(GetPawn()->GetActorLocation(), 
                                     CursorHit.Location);
  if (Distance > MaxHighlightDistance) return;
  ```

  ##### **视觉效果增强**
  ```cpp
  // 可添加淡入淡出效果
  void CursorTrace()
  {
      // 当前实现...
      
      // 扩展：根据距离调整高亮强度
      if (ThisActor)
      {
          float Intensity = CalculateHighlightIntensity(CursorHit.Distance);
          ThisActor->HighlightWithIntensity(Intensity);
      }
  }
  ```

  ##### **多目标支持**
  ```cpp
  // 未来可扩展为区域选择
  TArray<TObjectPtr<IEnemyInterface>> HighlightedActors;
  void HighlightArea(FVector Center, float Radius);
  ```

---

  #### 📊 **与其他系统集成**

  ```mermaid
  flowchart TD
      Input[鼠标移动事件] --> Tick[PlayerTick调用]
      Tick --> Trace[CursorTrace光线检测]
      Trace --> Interface[获取敌人接口]
      Interface --> Check[状态检查]
      
      Check -->|新敌人| Highlight[调用高亮接口]
      Check -->|离开敌人| Unhighlight[调用取消高亮接口]
      Check -->|同一敌人| NoAction[无操作]
      
      Highlight -->|触发| Enemy[敌人视觉效果]
      Unhighlight -->|触发| Enemy
  ```

---

  #### ⚠️ **注意事项**

  ##### **性能考虑**
  - **每帧调用**：确保 `CursorTrace` 逻辑轻量
  - **射线检测**：使用合适的碰撞通道和复杂度
  - **接口调用**：虚函数调用有一定开销

  ##### **网络同步**
  ```cpp
  // 如果是多人游戏，需要考虑
  bReplicates = true;  // 已启用复制
  // 但高亮效果可能需要服务器验证
  ```

  ##### **用户体验**
  - **响应速度**：立即反馈鼠标悬停
  - **视觉清晰**：高亮效果明显但不刺眼
  - **逻辑一致**：确保不会同时高亮多个敌人

---

  **核心改进**：通过每帧的鼠标射线检测，实现了智能的敌人高亮系统，为后续的敌人选择和战斗交互奠定了基础。

- 

  Post Process Highlight

  13:19
  
### 👾 **AuraEnemy 敌人实现类**

  #### **多重继承结构**
  ```cpp
  class AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
  ```
  - **角色基类**：继承通用的角色功能（武器、移动等）
  - **敌人接口**：实现高亮交互能力

  #### **碰撞通道配置**
  ```cpp
  GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
  GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
  ```
  - **可见性通道阻挡**：确保光标射线能检测到敌人
  - **相机通道忽略**：防止相机与敌人网格体碰撞

  #### **自定义深度高亮实现**
  ```cpp
  GetMesh()->SetRenderCustomDepth(true);
  GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
  ```
  - **渲染通道**：使用自定义深度渲染实现轮廓高亮
  - **模板值**：`CUSTOM_DEPTH_RED` 定义高亮颜色（通常在项目头文件中定义）
  - **武器同步**：武器组件也应用相同的高亮效果

  **核心**：通过UE的渲染系统实现视觉反馈，将接口逻辑转化为具体的视觉效果。



### 📚 第一章关键语法总结

#### 基础类定义
```cpp
UCLASS(Abstract)                     // 声明为抽象类，不可直接实例化
class GAS_AURA_API AAuraCharacterBase : public ACharacter
GENERATED_BODY()                     // UE反射系统必需宏
UPROPERTY(EditAnywhere, Category="Input") // 编辑器可见属性
TObjectPtr<USkeletalMeshComponent> Weapon  // UE5安全指针
```

#### 组件创建与设置
```cpp
PrimaryActorTick.bCanEverTick = false;   // 禁用Actor Tick
CreateDefaultSubobject<USkeletalMeshComponent>("Weapon"); // 创建组件
SetupAttachment(GetMesh(), FName("WeaponHandSocket"));    // 绑定到骨骼
SetCollisionEnabled(ECollisionEnabled::NoCollision);      // 禁用碰撞
```

#### 玩家控制器配置
```cpp
bReplicates = true;                              // 启用网络复制
Subsystem->AddMappingContext(AuraContext, 0);     // 添加输入映射（0=优先级）
bShowMouseCursor = true;                          // 显示鼠标光标
SetInputMode(InputModeData);                      // 设置输入模式
```

#### 增强输入系统
```cpp
UEnhancedInputLocalPlayerSubsystem* Subsystem = 
    ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
EnhancedInputComponent->BindAction(
    MoveAction,                    // 输入动作资源
    ETriggerEvent::Triggered,      // 触发事件类型
    this,                          // 目标对象
    &AAuraPlayerController::Move   // 回调函数指针
);
```

#### 角色移动处理
```cpp
const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();  // 获取2D输入
FRotator YawRotation(0.f, Rotation.Yaw, 0.f);     // 仅保留偏航角
FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // 获取前向向量
AddMovementInput(ForwardDirection, InputAxisVector.Y); // 应用移动输入
```

#### 角色运动组件配置
```cpp
GetCharacterMovement()->bOrientRotationToMovement = true;  // 朝向移动方向
GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f); // 旋转速度
GetCharacterMovement()->bConstrainToPlane = true;          // 平面约束
bUseControllerRotationYaw = false;                         // 禁用控制器旋转影响
```

#### 接口系统
```cpp
UINTERFACE(MinimalAPI)                      // 声明反射接口类
class UEnemyInterface : public UInterface   // 反射接口类
class IEnemyInterface                        // 功能接口类
virtual void HighlightActor() = 0;          // 纯虚函数声明（必须实现）
```

#### 光标追踪与接口调用
```cpp
GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);  // 光标射线检测
Cast<IEnemyInterface>(CursorHit.GetActor());               // 接口类型转换
LastActor->UnHighlightActor();                              // 调用接口方法
ThisActor->HighlightActor();                                // 调用接口方法
```

#### 自定义深度渲染
```cpp
SetRenderCustomDepth(true);                    // 启用自定义深度渲染
SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);  // 设置模板值（高亮颜色）
SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);  // 可见性碰撞响应
SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);     // 相机碰撞忽略
```

#### 状态管理逻辑
```cpp
if (LastActor != nullptr && LastActor != ThisActor)   // 需要取消高亮的情况
if (ThisActor != nullptr && ThisActor != LastActor)   // 需要高亮的情况
```

## 2.Intro to the Gameplay Ability System / 游戏技能系统介绍

- 8个讲座·1小时 23 分钟 / 8 Lectures · 1 Hour 23 Minutes

- The Gameplay Ability System

  12:14

- 

  The Main Parts of GAS

  08:46

- 

  The Player State

  04:28

### 🎮 AuraPlayerState 玩家状态类

  #### **类定义**
  ```cpp
  UCLASS()
  class GAS_AURA_API AAuraPlayerState : public APlayerState
  ```
  - **继承自** `APlayerState`：UE内置玩家状态基类
  - **功能**：存储玩家游戏数据（经验值、等级、属性点等）

  #### **构造函数配置**
  ```cpp
  AAuraPlayerState::AAuraPlayerState()
  {
      NetUpdateFrequency = 100.f;  // 设置网络更新频率为100Hz
  }
  ```

  #### **关键参数解析**
  | 参数                      | 类型    | 默认值 | 功能说明                 |
  | ------------------------- | ------- | ------ | ------------------------ |
  | `NetUpdateFrequency`      | `float` | `2.0f` | 网络更新频率（单位：Hz） |
  | `MinNetUpdateFrequency`   | `float` | `2.0f` | 最小更新频率             |
  | `bReplicateRelevancyInfo` | `bool`  | `true` | 是否复制相关性信息       |

  #### **NetUpdateFrequency 详解**
  ```cpp
  // 不同场景的设置建议
  NetUpdateFrequency = 100.f;  // 高频：MOBA、射击游戏
  NetUpdateFrequency = 30.f;   // 中频：RPG、动作游戏
  NetUpdateFrequency = 2.f;    // 低频：棋牌、策略游戏（默认值）
  
  // 实际更新间隔 = 1 / NetUpdateFrequency
  // 100Hz → 每0.01秒更新一次
  // 30Hz  → 每0.033秒更新一次
  // 2Hz   → 每0.5秒更新一次
  ```

  #### **网络同步流程**
  ```
  服务器端 PlayerState
      ↓ 网络复制（100Hz）
  客户端 PlayerState
      ↓
  更新玩家UI、属性显示等
  ```

  #### **典型应用场景**
  ```cpp
  // 存储玩家数据
  UPROPERTY(Replicated)
  int32 PlayerLevel;          // 玩家等级
  
  UPROPERTY(Replicated)
  float ExperiencePoints;     // 经验值
  
  UPROPERTY(Replicated)
  int32 AttributePoints;      // 属性点数
  
  // 后续可扩展
  UPROPERTY(Replicated)
  FString PlayerName;         // 玩家名称
  
  UPROPERTY(Replicated)
  int32 Gold;                 // 金币数量
  ```

  #### **设计考虑**
  1. **高频更新**：确保属性变化即时同步
  2. **带宽控制**：100Hz比默认2Hz消耗更多带宽
  3. **游戏类型适配**：根据需求调整频率
  4. **GAS集成**：为后续Gameplay Ability System做准备

  **核心作用**：作为玩家数据的网络同步载体，为RPG系统提供基础支持。

- 

  Ability System Component and Attribute Set

  05:08

- 

  GAS in Multiplayer

  10:29

- 

  Constructing the ASC and AS

  12:13

### 🏗️ **GAS 架构设计：分离式组件挂载**

  #### **核心架构对比**

  | 组件         | 玩家角色             | 敌人角色           | 设计原因               |
  | ------------ | -------------------- | ------------------ | ---------------------- |
  | **ASC**      | 挂载在 `PlayerState` | 挂载在 `EnemyBase` | 玩家数据需要跨关卡保存 |
  | **AS**       | 挂载在 `PlayerState` | 挂载在 `EnemyBase` | 属性与角色生命期绑定   |
  | **网络复制** | `PlayerState` 复制   | `Enemy` 自身复制   | 玩家状态持久化需求     |

  #### **1. 抽象基类设计**

  ##### **接口实现**
  ```cpp
  // 头文件：AuraCharacterBase.h
  class AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface
  {
      GENERATED_BODY()
  public:
      virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
      UAttributeSet* GetAttributeSet() const { return AttributeSet; };
      
  protected:
      UPROPERTY()
      TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;  // ASC指针
  
      UPROPERTY()
      TObjectPtr<UAttributeSet> AttributeSet;  // 属性集指针
  };
  ```

  ##### **接口方法实现**
  ```cpp
  // 源文件：AuraCharacterBase.cpp
  UAbilitySystemComponent* AAuraCharacterBase::GetAbilitySystemComponent() const
  {
      return AbilitySystemComponent;  // 返回ASC指针
  }
  ```

  #### **2. 玩家实现：PlayerState 挂载**

  ##### **构造函数初始化**
  ```cpp
  // AuraPlayerState.cpp
  AAuraPlayerState::AAuraPlayerState()
  {
      // 创建ASC组件
      AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
      AbilitySystemComponent->SetIsReplicated(true);  // 启用网络复制
      
      // 创建属性集
      AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("Attributeset");
      
      NetUpdateFrequency = 100.f;  // 高频网络更新
  }
  ```

  ##### **玩家角色获取ASC**
  ```cpp
  // 玩家角色需要通过PlayerState获取ASC
  AAuraPlayerState* PlayerState = GetPlayerState<AAuraPlayerState>();
  if (PlayerState)
  {
      UAbilitySystemComponent* ASC = PlayerState->GetAbilitySystemComponent();
      UAttributeSet* AS = PlayerState->GetAttributeSet();
  }
  ```

  #### **3. 敌人实现：Enemy 自身挂载**

  ##### **构造函数初始化**
  ```cpp
  // AuraEnemy.cpp
  AAuraEnemy::AAuraEnemy()
  {
      // 碰撞设置（保持不变）
      GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
      GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
      
      // 创建ASC组件（直接挂载到敌人）
      AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
      AbilitySystemComponent->SetIsReplicated(true);  // 启用网络复制
      
      // 创建属性集
      AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("Attributeset");
  }
  ```

  ##### **敌人角色获取ASC**
  ```cpp
  // 敌人可以直接从自身获取ASC
  UAbilitySystemComponent* ASC = GetAbilitySystemComponent();  // 继承自基类
  UAttributeSet* AS = GetAttributeSet();                       // 基类方法
  ```

  #### **4. 设计哲学分析**

  ##### **玩家数据持久化**
  ```cpp
  // PlayerState的生命周期
  进入游戏 → 创建PlayerState → 存储ASC/AS数据
  死亡重生 → PlayerState保留 → 数据不丢失
  退出关卡 → PlayerState销毁 → 需要保存到存档
  ```

  ##### **敌人临时性**
  ```cpp
  // Enemy的生命周期
  关卡开始 → 生成敌人 → 创建ASC/AS
  玩家击杀 → 敌人销毁 → ASC/AS同时销毁
  关卡结束 → 所有敌人销毁 → 无需保存
  ```

  #### **5. 网络复制策略**

  ##### **玩家复制模式**
  ```cpp
  // PlayerState中设置
  AbilitySystemComponent->SetIsReplicated(true);
  // 通常使用：Mixed（混合）或 Full（完全）复制
  AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
  ```

  ##### **敌人复制模式**
  ```cpp
  // Enemy中设置（相同）
  AbilitySystemComponent->SetIsReplicated(true);
  // 通常使用：Minimal（最小）复制节省带宽
  AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
  ```

  #### **6. 访问模式对比**

  | 操作         | 玩家代码                                        | 敌人代码                      |
  | ------------ | ----------------------------------------------- | ----------------------------- |
  | **获取ASC**  | `GetPlayerState()->GetAbilitySystemComponent()` | `GetAbilitySystemComponent()` |
  | **获取AS**   | `GetPlayerState()->GetAttributeSet()`           | `GetAttributeSet()`           |
  | **应用效果** | 通过PlayerState的ASC                            | 直接通过自身ASC               |
  | **监听属性** | 监听PlayerState的AS                             | 监听自身AS                    |

  #### **7. 优势和考量**

  ##### **优势**
  1. **数据分离**：玩家进度与角色实体解耦
  2. **持久化**：玩家死亡/重生不丢失属性
  3. **网络优化**：PlayerState可独立复制频率
  4. **存档友好**：PlayerState数据易于序列化

  ##### **考量**
  1. **访问路径**：玩家需通过PlayerState访问，增加间接性
  2. **初始化时机**：需确保PlayerState在角色之前创建
  3. **引用管理**：注意PlayerState与角色的生命周期差异

  #### **总结**
  这种**分离式架构**是多人RPG游戏的**最佳实践**：
  - **玩家**：`PlayerState`作为数据容器，支持进度保存
  - **敌人**：`Enemy`自身承载数据，简化生命周期管理

  为后续的**属性系统**、**技能系统**和**伤害计算**奠定了坚实的架构基础。

- 

  Replication Mode

  07:44

### 🌐 **GAS 网络复制模式配置**

  #### **1. 玩家状态：Mixed 复制模式**
  ```cpp
  // PlayerState 构造函数新增
  AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
  ```

  ##### **Mixed 模式特点**
  - **本地玩家**：接收完整的GameplayEffect数据
  - **其他玩家**：只接收最小必要数据
  - **适用场景**：玩家自己的角色（需要完整数据），AI控制的角色用Minimal

  #### **2. 敌人：Minimal 复制模式**
  ```cpp
  // Enemy 构造函数新增  
  AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
  ```

  ##### **Minimal 模式特点**
  - **只复制**：GameplayTags和持续时间
  - **不复制**：具体的属性修改值
  - **适用场景**：AI敌人、小兵（带宽优化）

  #### **3. 复制模式对比表
| 复制模式        | 使用场景           | 描述                                                         |
| --------------- | ------------------ | ------------------------------------------------------------ |
| Full（完整）    | 单人游戏           | Gameplay Effects 复制到所有客户端                            |
| Mixed（混合）   | 多人游戏，玩家控制 | Gameplay Effects 仅复制到所属客户端。Gameplay Cues 和 Gameplay Tags 复制到所有客户端。 |
| Minimal（最小） | 多人游戏，AI控制   | Gameplay Effects 不进行复制。Gameplay Cues 和 Gameplay Tags 复制到所有客户端。 |

  #### **设计考虑**

  1. **网络优化**：根据角色重要性选择复制模式
  2. **带宽控制**：敌人用Minimal节省服务器资源  
  3. **玩家体验**：本地玩家需要完整数据计算伤害等
  4. **一致性**：保持接口实现的一致性

- 

  Init Ability Actor Info

  22:01
  
### 🎮 **GAS 初始化系统**

  #### **1. 敌人角色初始化**
  ```cpp
  // AuraEnemy.cpp - BeginPlay()
  void AAuraEnemy::BeginPlay()
  {
      Super::BeginPlay();
      AbilitySystemComponent->InitAbilityActorInfo(this, this);
  }
  ```

  ##### **InitAbilityActorInfo 参数**
  ```cpp
  InitAbilityActorInfo(
      this,  // 第1参数：OwnerActor（拥有者）
      this   // 第2参数：AvatarActor（化身）
  );
  ```
  - **拥有者**：逻辑上的所有者（敌人自身）
  - **化身**：实际执行动作的实体（敌人自身）
  - **敌人场景**：Owner和Avatar都是敌人自己

  #### **2. 玩家角色初始化**

  ##### **服务器端初始化**
  ```cpp
  // AuraCharacter.cpp - PossessedBy()
  void AAuraCharacter::PossessedBy(AController* NewController)
  {
      Super::PossessedBy(NewController);
      InitAbilityActorInfo();  // 服务器初始化
  }
  ```
  - **触发时机**：服务器获得角色控制权时
  - **典型场景**：玩家加入游戏、重生时

  ##### **客户端初始化**
  ```cpp
  // AuraCharacter.cpp - OnRep_PlayerState()
  void AAuraCharacter::OnRep_PlayerState()
  {
      Super::OnRep_PlayerState();
      InitAbilityActorInfo();  // 客户端初始化
  }
  ```
  - **触发时机**：客户端PlayerState同步完成时
  - **网络复制**：通过`OnRep_PlayerState`响应复制事件

  #### **3. 玩家ASC初始化实现**
  ```cpp
  // AuraCharacter.cpp - InitAbilityActorInfo()
  void AAuraCharacter::InitAbilityActorInfo()
  {
      // 获取PlayerState
      AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
      check(AuraPlayerState);  // 断言验证
      
      // 初始化ASC
      AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(
          AuraPlayerState,  // Owner：PlayerState
          this              // Avatar：角色实体
      );
      
      // 保存引用
      AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
      AttributeSet = AuraPlayerState->GetAttributeSet();
  }
  ```

  ##### **玩家参数对比**
  ```cpp
  InitAbilityActorInfo(
      AuraPlayerState,  // Owner：PlayerState（数据持久化）
      this              // Avatar：角色实体（执行动作）
  );
  ```

  #### **4. 初始化时机对比表**

  | 角色类型 | 服务器初始化    | 客户端初始化          | Owner       | Avatar   |
  | -------- | --------------- | --------------------- | ----------- | -------- |
  | **玩家** | `PossessedBy()` | `OnRep_PlayerState()` | PlayerState | 角色     |
  | **敌人** | `BeginPlay()`   | `BeginPlay()`         | 敌人自身    | 敌人自身 |

  #### **5. 关键函数作用**

  ##### **InitAbilityActorInfo()**
  ```cpp
  // GAS核心初始化函数
  ASC->InitAbilityActorInfo(OwnerActor, AvatarActor);
  ```
  - **绑定关系**：建立Owner、Avatar与ASC的关联
  - **激活系统**：使GAS开始工作
  - **必需调用**：未调用则技能系统无法使用

  ##### **check() 宏**
  ```cpp
  check(AuraPlayerState);  // 开发时验证，失败则崩溃
  ```
  - **调试辅助**：确保关键对象存在
  - **发布版本**：自动移除，不影响性能

  #### **设计模式总结**
  - **玩家**：分离式初始化（Owner=PlayerState, Avatar=角色）
  - **敌人**：一体化初始化（Owner=Avatar=敌人自身）
  - **网络同步**：确保两端都正确初始化
  - **生命周期**：在合适的时机触发初始化

### 📚 第二章关键语法总结

#### GAS核心组件定义
```cpp
UCLASS()
class GAS_AURA_API AAuraPlayerState : public APlayerState  // 玩家状态类
```

#### 网络更新频率设置
```cpp
NetUpdateFrequency = 100.f;  // 设置网络更新频率为100Hz（默认2Hz）
```

#### GAS接口实现
```cpp
class AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface  // 继承GAS接口
virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;  // 实现接口方法
```

#### ASC（能力系统组件）创建
```cpp
AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
AbilitySystemComponent->SetIsReplicated(true);  // 启用网络复制
```

#### AS（属性集）创建
```cpp
AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
```

#### GAS复制模式设置
```cpp
AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);     // 玩家：混合模式
AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);   // 敌人：最小模式
```

#### GAS初始化系统调用
```cpp
ASC->InitAbilityActorInfo(OwnerActor, AvatarActor);  // 核心初始化函数
```

#### 玩家初始化时机函数
```cpp
virtual void PossessedBy(AController* NewController) override;  // 服务器端初始化
virtual void OnRep_PlayerState() override;                      // 客户端初始化
```

#### 断言验证宏
```cpp
check(AuraPlayerState);  // 开发时验证对象有效性
```

#### 复制模式枚举值
```cpp
EGameplayEffectReplicationMode::Full      // 完整复制（单人游戏）
EGameplayEffectReplicationMode::Mixed     // 混合复制（玩家控制）
EGameplayEffectReplicationMode::Minimal   // 最小复制（AI控制）
```

#### 玩家状态获取
```cpp
AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();  // 获取玩家状态
```

#### 组件引用保存
```cpp
AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();  // 保存ASC引用
AttributeSet = AuraPlayerState->GetAttributeSet();                      // 保存AS引用
```

## 3.Attributes / 属性

- 4个讲座·1小时1分钟 / 4 Lectures · 1 Hour 1 Minute

- Attributes

  06:59

> Attributes are  numerical quantities associated with a given entity in the game, all attributes are floats, they exist within a structure called FGameplayAttributeData.
>
> 属性是与游戏中特定实体相关联的数值量，所有属性均为浮点数，它们存在于名为FGameplayAttributeData的结构中。

- 

  Health and Mana

  17:44

### 🏥 **AuraAttributeSet 属性集实现**

  #### **1. 类定义和属性声明**
  ```cpp
  UCLASS()
  class GAS_AURA_API UAuraAttributeSet : public UAttributeSet
  {
      GENERATED_BODY()
      
  public:
      UAuraAttributeSet();
      
      virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
      // TArray<FLifetimeProperty>& OutLifetimeProps
  	// 参数：输出参数，存放所有需要复制的属性信息
  	// 类型：TArray（动态数组），存储FLifetimeProperty结构
      
      // 核心属性声明
      UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
      FGameplayAttributeData Health;
      
      UFUNCTION()
      void OnRep_Health(const FGameplayAttributeData& OldHealth) const;
  };
  ```

  ##### **代码功能**
    1. **继承UAttributeSet**：GAS属性系统的基类
    2. **网络复制配置函数**GetLifetimeReplicatedProps：告诉UE哪些属性需要网络复制
    3. **声明Health属性**：使用`FGameplayAttributeData`类型存储生命值
    4. **声明OnRep_Health函数**：属性复制完成时的回调函数

  #### **2. 构造函数实现**
  ```cpp
  UAuraAttributeSet::UAuraAttributeSet()
  {
      // 空的构造函数，属性初始化使用默认值
  }
  ```

  #### **3. 网络复制配置**
  ```cpp
  void UAuraAttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
  {
      // 1. 首先调用父类方法
      Super::GetLifetimeReplicatedProps(OutLifetimeProps);
      
      // 2. 注册Health属性进行网络复制
      DOREPLIFETIME_CONDITION_NOTIFY(UAuraAttributeSet, Health, COND_None, REPNOTIFY_Always);
  }
  ```

  ##### **DOREPLIFETIME_CONDITION_NOTIFY宏解析**
  ```cpp
  DOREPLIFETIME_CONDITION_NOTIFY(
      UAuraAttributeSet,   // 参数1：当前类名
      Health,              // 参数2：要复制的属性名
      COND_None,           // 参数3：复制条件（无条件，总是复制）
      REPNOTIFY_Always     // 参数4：通知策略（总是发送通知）
  )
  ```

  ##### **作用**
  - 告诉UE：`Health`属性需要通过网络同步
  - 条件`COND_None`：任何情况下都复制
  - 通知`REPNOTIFY_Always`：属性变化时总是通知

  #### **4. OnRep函数实现**
  ```cpp
  void UAuraAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
  {
      GAMEPLAYATTRIBUTE_REPNOTIFY(UAuraAttributeSet, Health, OldHealth);
  }
  ```

  ##### **GAMEPLAYATTRIBUTE_REPNOTIFY宏功能**
  ```cpp
  // 这个宏内部执行三个操作：
  // 1. 比较新旧值，触发属性变化事件
  // 2. 更新UI显示（如果绑定了UI）
  // 3. 确保属性值正确同步
  ```

  ##### **OnRep_Health参数**
  ```cpp
  const FGameplayAttributeData& OldHealth  // 参数：属性复制前的旧值
  ```

  #### **5. 完整执行流程**

  ##### **服务器端发生属性变化**
  ```
  服务器：
  1. Health属性值改变（例如：玩家受伤）
  2. 自动触发网络复制系统
  3. 通过网络发送Health新值给客户端
  ```

  ##### **客户端接收属性变化**
  ```
  客户端：
  1. 收到服务器发来的Health新值
  2. UE自动调用OnRep_Health(OldHealth)
  3. GAMEPLAYATTRIBUTE_REPNOTIFY宏执行
     - 记录旧值
     - 更新新值
     - 触发属性变化事件
  4. UI系统收到事件，更新生命条显示
  ```

  #### **6. 代码结构总结**
  ```cpp
  // 每个属性需要三部分：
  1. 声明属性：UPROPERTY(...) FGameplayAttributeData 属性名;
  2. 复制注册：DOREPLIFETIME_CONDITION_NOTIFY(类名, 属性名, 条件, 通知策略);
  3. OnRep函数：void OnRep_属性名(const FGameplayAttributeData& Old值) const;
  
  // 对应关系：
  Health属性  ←→  OnRep_Health函数  ←→  DOREPLIFETIME_CONDITION_NOTIFY注册
  ```

  #### **7. 核心机制**
  - **网络复制**：服务器向客户端同步属性值
  - **回调通知**：属性复制完成后调用指定函数
  - **自动同步**：GAS系统自动处理属性变化和UI更新

- 

  Attribute Accessors

  12:05

### 🚀 **AuraAttributeSet 属性集升级**

  #### **1. 新增：属性访问器宏定义**
  ```cpp
  #define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
      GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
      GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
      GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
      GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
  ```

  ##### **宏展开后的效果**
  ```cpp
  // 以Health为例，ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health) 展开为：
      
      // 1. 属性元数据获取器
      static FGameplayAttribute GetHealthAttribute();
      
      // 2. 当前值获取器
      float GetHealth() const;
      
      // 3. 当前值设置器
      void SetHealth(float NewVal);
      
      // 4. 初始值设置器
      void InitHealth(float NewVal);
  ```

  #### **2. 属性声明新增宏调用**
  ```cpp
  UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Vital Attributes")
  FGameplayAttributeData Health;
  ATTRIBUTE_ACCESSORS(UAuraAttributeSet, Health);  // 新增：为Health生成访问函数
  ```

  #### **3. 构造函数初始化属性值**
  ```cpp
  UAuraAttributeSet::UAuraAttributeSet()
  {
      // 使用宏生成的Init函数设置初始值
      InitHealth(100.f);      // Health初始值 = 100
      InitMaxHealth(100.f);   // MaxHealth初始值 = 100
      InitMana(50.f);         // Mana初始值 = 50
      InitMaxMana(50.f);      // MaxMana初始值 = 50
  }
  ```

  ##### **Init函数来源**
  ```cpp
  // 来自宏：GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
  // 生成函数：InitHealth(float), InitMaxHealth(float)等
  ```

  #### **4. 生成的访问函数使用示例**

  ##### **获取属性值**
  ```cpp
  // 通过宏生成的Get函数
  float CurrentHealth = GetHealth();        // 获取当前生命值
  float CurrentMaxHealth = GetMaxHealth();  // 获取最大生命值
  
  // 之前的方法（不方便）
  float OldWay = Health.GetCurrentValue();  // 需要调用GetCurrentValue()
  ```

  ##### **设置属性值**
  ```cpp
  // 通过宏生成的Set函数
  SetHealth(75.f);    // 设置生命值为75
  SetMana(30.f);      // 设置魔法值为30
  
  // 之前的方法（复杂）
  FGameplayAttributeData NewHealth;
  NewHealth.SetCurrentValue(75.f);
  Health = NewHealth;
  ```

  #### **5. 宏的功能详解**

  | 宏组件                              | 生成的函数             | 作用                             |
  | ----------------------------------- | ---------------------- | -------------------------------- |
  | `GAMEPLAYATTRIBUTE_PROPERTY_GETTER` | `GetHealthAttribute()` | 获取属性的元数据（类型、名称等） |
  | `GAMEPLAYATTRIBUTE_VALUE_GETTER`    | `GetHealth()`          | 获取属性的当前值（float）        |
  | `GAMEPLAYATTRIBUTE_VALUE_SETTER`    | `SetHealth(float)`     | 设置属性的当前值                 |
  | `GAMEPLAYATTRIBUTE_VALUE_INITTER`   | `InitHealth(float)`    | 初始化属性的基础值               |

  #### **6. 代码结构对比**

  ##### **升级前（手动管理）**
  ```cpp
  // 获取值
  float health = Health.GetCurrentValue();
  
  // 设置值（复杂）
  FGameplayAttributeData newHealth;
  newHealth.SetCurrentValue(100.f);
  Health = newHealth;
  
  // 没有统一的初始化方法
  ```

  ##### **升级后（宏辅助）**
  ```cpp
  // 获取值（简洁）
  float health = GetHealth();
  
  // 设置值（简单）
  SetHealth(100.f);
  
  // 初始化（统一）
  InitHealth(100.f);  // 构造函数中调用
  ```

  #### **7. 关键改进总结**
  1. **代码简化**：宏自动生成Get/Set/Init函数
  2. **类型安全**：统一的访问接口
  3. **初始化标准化**：构造函数中统一初始化所有属性
  4. **可维护性**：属性声明和访问函数绑定在一起

  ##### **使用新宏的优势**
  ```cpp
  // 之前：手动编写每个属性的访问函数
  // 之后：一行宏搞定所有功能
  // 结果：减少代码量，提高一致性，减少错误
  ```

- 

  Effect Actor

  24:20

###  **⚠️ AuraEffectActor 临时效果实现与const_cast问题**

  #### **1. Actor基础结构**
  ```cpp
  UCLASS()
  class GAS_AURA_API AAuraEffectActor : public AActor
  {
      // 组件
      TObjectPtr<UStaticMeshComponent> Mesh;    // 可视网格
      TObjectPtr<USphereComponent> Sphere;      // 碰撞检测球体
      
      // 碰撞回调函数
      virtual void OnOverlap(...);  // 进入碰撞区域
      virtual void EndOverlap(...); // 离开碰撞区域
  };
  ```

  #### **2. 当前的临时解决方案**
  ```cpp
  void AAuraEffectActor::OnOverlap(...)
  {
      // 1. 检查OtherActor是否实现了IAbilitySystemInterface
      if(IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
      {
          // 2. 获取OtherActor的属性集
          const UAuraAttributeSet* AuraAttributeSet = 
              Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()
                  ->GetAttributeSet(UAuraAttributeSet::StaticClass()));
          
          // 3. ⚠️ 使用const_cast移除const修饰符（危险操作）
          UAuraAttributeSet* MutableAuraAttributeSet = 
              const_cast<UAuraAttributeSet*>(AuraAttributeSet);
          
          // 4. 直接修改生命值
          MutableAuraAttributeSet->SetHealth(AuraAttributeSet->GetHealth() + 25.f);
          
          // 5. 销毁自身
          Destroy();
      }
  }
  ```

  #### **3. const_cast的问题分析**

  ##### **什么是const_cast？**
  ```cpp
  // const_cast语法：移除或添加const修饰符
  const_cast<Type*>(const_pointer);  // 移除const
  const_cast<const Type*>(pointer);  // 添加const
  
  // 当前代码：
  const UAuraAttributeSet* AuraAttributeSet = ...;  // const指针
  UAuraAttributeSet* MutableAuraAttributeSet =       // 移除const
      const_cast<UAuraAttributeSet*>(AuraAttributeSet);
  ```

  ##### **const_cast的严重问题**

  **问题1：违反const承诺**
  ```cpp
  // GetAttributeSet返回const指针的承诺：
  // "这个对象是只读的，我不会修改它"
  const UAttributeSet* GetAttributeSet(...) const;
  
  // 使用const_cast打破了这个承诺
  // 可能导致：
  // 1. 其他代码依赖const保证，现在被破坏
  // 2. 多线程环境下的数据竞争
  ```

  **问题2：绕过GAS系统**
  ```cpp
  // GAS正确的属性修改方式：
  ASC->ApplyModToAttribute(Attribute, Modifier);  // 通过ASC系统
  
  // 当前错误方式：
  直接调用 SetHealth()  // 绕过GAS，不会触发：
  // - 属性变化事件
  // - UI更新
  // - 网络复制
  // - GameplayEffect的后续处理
  ```

  **问题3：网络同步问题**
  ```cpp
  // GAS修改属性会：
  1. 服务器修改 → 2. 触发复制 → 3. 客户端同步
  
  // const_cast直接修改：
  1. 本地修改 → 2. 网络不同步 → 3. 其他客户端看不到变化
  ```

  #### **4. 总结：为什么const_cast是坏的**

  | 问题              | 后果                         | 正确做法                  |
  | ----------------- | ---------------------------- | ------------------------- |
  | **违反const约定** | 破坏代码安全性，可能导致崩溃 | 使用const正确的方法       |
  | **绕过GAS系统**   | 不触发事件、UI不更新         | 通过ASC应用GameplayEffect |
  | **网络不同步**    | 多人游戏不同步               | GAS自动处理网络复制       |
  | **代码维护困难**  | 难以调试和追踪               | 使用标准GAS流程           |

  **注释中的TODO：**
  ```cpp
  //TODO: 将此更改为应用游戏效果，目前使用 const_cast 作为临时解决方案！
  // 翻译：这只是临时方案，后面要用GameplayEffect重写！
  ```

- 

  Section 4 Quiz

  3 问题

## 4.RPG Game UI / RPG游戏用户界面

- 9 个讲座·2 小时 27 分钟 / 9 Lectures · 2 Hours 27 Minutes

- Game UI Architecture

  07:36

- 

  Aura User Widget and Widget Controller

  10:39

- 

  Globe Progress Bar

  28:38

- 

  Health Globe

  10:38

- 

  Aura HUD

  08:14

- 

  Overlay Widget Controller

  32:15

- 

  Broadcasting Initial Values

  25:37

- 

  Listening for Attribute Changes

  11:58

- 

  Callbacks for Mana Changes

  11:02

## 5.Gameplay Effects / 游戏效果

- 12 个讲座·3 小时 31分钟 / 12 Lectures · 3 Hours 31 Minutes

- Gameplay Effects

  07:41

- 

  Effect Actor Improved

  29:48

- 

  Instant Gameplay Effects

  20:26

- 

  Duration Gameplay Effects

  18:21

- 

  Periodic Gameplay Effects

  16:17

- 

  Effect Stacking

  14:36

- 

  Infinite Gameplay Effects

  12:55

- 

  Instant and Duration Application Policy

  04:16

- 

  Infinite Effect Application and Removal

  28:37

- 

  PreAttributeChange

  12:16

- 

  PostGameplayEffectExecute

  30:57

- 

  Curve Tables for Scalable Floats

  15:19

## 6.Gameplay Tags / 游戏标签

- 16 个讲座·3 小时 23 分钟 / 16 Lectures · 3 Hours 23 Minutes

- Gameplay Tags

  05:55

- 

  Creating Gameplay Tags in the Editor

  07:42

- 

  Creating Gameplay Tags from Data Tables

  06:22

- 

  Adding Gameplay Tags to Gameplay Effects

  00:15

- 

  Apply Gameplay Tags with Effects

  18:18

- 

  Gameplay Effect Delegates

  12:46

- 

  Get All Asset Tags

  10:34

- 

  Broadcasting Effect Asset Tags

  10:03

- 

  UI Widget Data Table

  12:40

- 

  Retrieving Rows from Data Tables

  16:17

- 

  Broadcasting Data Table Rows

  15:49

- 

  Message Widget

  15:24

- 

  Animating the Message Widget

  17:38

- 

  Replacing Callbacks with Lambdas

  09:44

- 

  Ghost Globe

  33:43

- 

  Properly Clamping Attributes

  09:41

## 7.RPG Attributes / RPG属性

- 11个讲座·2 小时 48 分钟 / 11 Lectures · 2 Hours 48 Minutes

- Initialize Attributes from a Data Table

  14:44

- 

  Initialize Attributes with Gameplay Effects

  15:52

- 

  Attribute Based Modifiers

  09:09

- 

  Modifier Order of Operations

  11:08

- 

  Modifier Coefficients

  06:54

- 

  Secondary Attributes

  13:47

- 

  Derived Attributes

  31:39

- 

  Custom Calculations

  06:14

- 

  Player Level and Combat Interface

  13:07

- 

  Modifier Magnitude Calculations

  37:27

- 

  Initializing Vital Attributes

  07:47

## 8.Attribute Menu / 属性菜单

- 21个讲座·4 小时 42 分钟 / 21 Lectures · 4 Hours 42 Minutes

- Attribute Menu - Game Plan

  05:20

- 

  Attribute Menu - Framed Value

  11:08

- 

  Attribute Menu - Text Value Row

  08:28

- 

  Attribute Menu - Text Value Button Row

  06:43

- 

  Attribute Menu - Construction

  18:43

- 

  Button Widget

  21:34

- 

  Wide Button Widget

  09:19

- 

  Opening the Attribute Menu

  07:06

- 

  Closing the Attribute Menu

  05:40

- 

  Plan for Displaying Attribute Data

  10:07

- 

  Gameplay Tags Singleton

  09:52

- 

  Aura Asset Manager

  14:48

- 

  Native Gameplay Tags

  09:10

- 

  Attribute Info Data Asset

  15:46

- 

  Attribute Menu Widget Controller

  05:17

- 

  Aura Ability System Blueprint Library

  15:05

- 

  Constructing the Attribute Menu Widget Controller

  17:07

- 

  Attribute Info Delegate

  20:30

- 

  Widget Attribute Tags

  16:01

- 

  Mapping Tags to Attributes

  37:58

- 

  Responding to Attribute Changes

  16:07

## 9.Gameplay Abilities / 游戏技能

- 15 个讲座·3 小时 56 分钟 / 15 Lectures · 3 Hours 56 Minutes

- Gameplay Abilities

  05:09

- 

  Granting Abilities

  15:07

- 

  Settings on Gameplay Abilities

  18:31

- 

  Input Config Data Asset

  22:19

- 

  Aura Input Component

  14:52

- 

  Callbacks for Ability Input

  12:46

- 

  Activating Abilities

  27:22

- 

  Click To Move

  16:23

- 

  Setting Up Click to Move

  18:17

- 

  Setting Up Auto Running

  11:48

- 

  Implementing Auto Running

  11:42

- 

  Code Clean Up

  11:29

- 

  Aura Projectile

  14:25

- 

  Aura Projectile Spell

  13:37

- 

  Spawning Projectiles

  22:33

## 10.Ability Tasks / 技能任务

- 15 个讲座·3 小时 53 分钟 / 15 Lectures · 3 Hours 53 Minutes

- Ability Tasks

  16:54

- 

  Sending Gameplay Events

  10:38

- 

  Spawn FireBolt from Event

  05:50

- 

  Custom Ability Tasks

  20:18

- 

  Target Data

  06:10

- 

  Send Mouse Cursor Data

  13:42

- 

  Receiving Target Data

  16:11

- 

  Prediction in GAS

  10:53

- 

  Orienting the Projectile

  16:55

- 

  Motion Warping

  15:16

- 

  Projectile Impact

  16:05

- 

  Projectile Collision Channel

  09:31

- 

  Projectile Gameplay Effect

  23:42

- 

  Enemy Health Bar

  30:23

- 

  Ghost Bar

  20:56

## 11.RPG Character Classes / RPG角色职业

- 5 个讲座·1小时 11分钟 / 5 Lectures · 1 Hour 11 Minutes

- RPG Character Classes

  06:24

- 

  Character Class Info

  07:27

- 

  Default Attribute Effects

  04:28

- 

  Curve Tables - CSV and JSON

  29:52

- 

  Initializing Enemy Attributes

  22:40

## 12.Damage / 伤害

- 17个讲座·4 小时 32 分钟 / 17 Lectures · 4 Hours 32 Minutes

- Meta Attributes

  04:29

- 

  Damage Meta Attribute

  08:44

- 

  Set By Caller Magnitude

  07:37

- 

  Ability Damage

  14:46

- 

  Enemy Hit React

  26:52

- 

  Activating the Enemy Hit React Ability

  17:32

- 

  Enemy Death

  12:49

- 

  Dissolve Effect

  22:19

- 

  Floating Text Widget

  15:03

- 

  Showing Damage Text

  25:24

- 

  Execution Calculations

  06:29

- 

  Damage Execution Calculation

  08:39

- 

  ExecCalcs - Capturing Attributes

  28:27

- 

  Implementing Block Chance

  18:30

- 

  Implementing Armor and Armor Penetration

  12:25

- 

  Damage Calculation Coefficients

  24:57

- 

  Implementing Critical Hits

  17:23

## 13.Advanced Damage Techniques / 高级伤害技术

- 14 个讲座·4 小时16 分钟 / 14 Lectures · 4 Hours 16 Minutes

- The Gameplay Effect Context

  34:48

- 

  Custom Gameplay Effect Context

  14:24

- 

  NetSerialize

  27:41

- 

  Implementing Net Serialize

  07:23

- 

  Struct Ops Type Traits

  07:46

- 

  Aura Ability System Globals

  11:17

- 

  Using a Custom Effect Context

  23:00

- 

  Floating Text Color

  19:23

- 

  Hit Message

  16:17

- 

  Damage Types

  20:25

- 

  Mapping Damage Types to Resistances

  10:23

- 

  Resistance Attributes

  21:30

- 

  Resistance Damage Reduction

  17:42

- 

  Multiplayer Test

  23:59

## 14.Enemy AI / 敌人AI

- 14 个讲座·2 小时 33 分钟 / 14 Lectures · 2 Hours 33 Minutes

- Enemy AI Setup

  04:43

- 

  AI Controller Blackboard and Behavior Tree

  15:41

- 

  Behavior Tree Service

  10:25

- 

  Blackboard Keys

  13:40

- 

  Finding the Nearest Player

  09:10

- 

  AI and Effect Actors

  15:12

- 

  Behavior Tree Decorators

  18:39

- 

  Attack Behavior Tree Task

  10:33

- 

  Find New Location Around Target

  14:43

- 

  Environment Query System

  03:24

- 

  Environment Queries

  09:29

- 

  EQS Tests

  09:59

- 

  Distance Test

  05:21

- 

  Using EQS Queries in Behavior Trees

  11:44

## 15.Enemy Melee Attacks / 敌人近战攻击

- 13 个讲座·3 小时8分钟 / 13 Lectures · 3 Hours 8 Minutes

- Melee Attack Ability

  24:41

- 

  Attack Montage

  08:05

- 

  Combat Target

  15:16

- 

  Melee Attack Gameplay Event

  16:12

- 

  Get Live Players Within Radius

  32:07

- 

  Causing Melee Damage

  15:31

- 

  Multiplayer Melee Test

  03:13

- 

  Montage Gameplay Tags

  07:26

- 

  Tagged Montage

  09:36

- 

  Multiple Attack Sockets

  09:10

- 

  Ghoul Enemy

  11:58

- 

  Ghoul Attack Montages

  12:49

- 

  Melee Polish

  21:56

## 16.Enemy Ranged Attacks / 敌人远程攻击

- 9 个讲座·1小时 15 分钟 / 9 Lectures · 1 Hour 15 Minutes

- Ranged Attack

  06:50

- 

  Rock Projectile

  03:21

- 

  Ranged Damage Curve

  03:47

- 

  Granting Ranged Attacks

  02:51

- 

  Slingshot Attack Montage

  05:20

- 

  Playing the Ranged Attack Montage

  13:53

- 

  Spawning the Rock Projectile

  14:03

- 

  Slingshot Animation Blueprint

  11:40

- 

  Slingshot Attack Montage

  13:11

## 17.Enemy Spell Attacks / 敌人法术攻击

- 5个讲座·38 分钟 / 5 Lectures · 38 Minutes

- Goblin Shaman

  07:31

- 

  Shaman Attack Montage

  04:06

- 

  Shaman Attack Ability

  10:33

- 

  Dead Blackboard Key

  07:18

- 

  Enemies Multiplayer Testing

  08:48

## 18.Enemy Finishing Touches / 敌人最终完善

- 26 个讲座·4 小时 33 分钟 / 26 Lectures · 4 Hours 33 Minutes

- Goblin Spear - Sound Notifies

  06:19

- 

  Impact Effects

  16:35

- 

  Melee Impact Gameplay Cue

  18:58

- 

  Montage and Socket Tags

  22:13

- 

  Goblin Spear - Hurt and Death Sounds

  07:19

- 

  Goblin Slingshot - Sound Notifies

  04:31

- 

  Rock Impact Effects

  07:01

- 

  Goblin Shaman - Sound Notifies

  04:11

- 

  Ghoul - Sound Notifies

  10:21

- 

  Ghoul - Swipe Trail

  04:48

- 

  Demon Blueprint

  05:55

- 

  Demon Melee Attack

  14:50

- 

  Demon Ranged Attack

  11:04

- 

  Demon - Sound Notifies

  09:43

- 

  Demon - Dissolve Effect

  01:56

- 

  Shaman Summon Locations

  28:23

- 

  Async Spawn Times

  10:34

- 

  Summoning Particle Effect

  04:54

- 

  Select Minion Class at Random

  06:01

- 

  Minion Summon Montage

  05:48

- 

  Minion Count

  07:34

- 

  Elementalist Behavior Tree

  09:35

- 

  Elementalist Attack Task

  14:11

- 

  Decrementing Minion Count

  12:18

- 

  Adding Juice with Tweening

  08:25

- 

  Enemies Final Polish

  19:42

## 19.Level Tweaks / 关卡调整

- 5 个讲座·1小时 54 分钟 / 5 Lectures · 1 Hour 54 Minutes

- Level Lighting and Post Process

  28:39

- 

  Texture Streaming Pool Over Budget

  20:41

- 

  Flame Pillar Actor

  13:23

- 

  Fade Actor

  24:49

- 

  Fading Out Obstructing Geometry

  26:00

## 20.Cost and Cooldown / 消耗与冷却

- 14 个讲座·3 小时 48 分钟 / 14 Lectures · 3 Hours 48 Minutes

- Health Mana Spells Widget

  16:19

- 

  Spell Globe

  20:04

- 

  Adding Spell Globes

  15:21

- 

  XP Bar

  10:25

- 

  Ability Info Data Asset

  20:01

- 

  Initialize Overlay Startup Abilities

  13:26

- 

  For Each Ability Delegate

  19:46

- 

  Binding Widget Events to the Ability Info Delegate

  18:55

- 

  Gameplay Ability Cost

  12:29

- 

  Gameplay Ability Cooldown

  09:22

- 

  Cooldown Async Task

  38:18

- 

  Cooldown Tags in Ability Info

  06:58

- 

  Showing Cooldown Time in the HUD

  13:43

- 

  Modeling Mode

  13:02

## 21.Experience and Leveling Up / 经验与升级

- 15 个讲座·3 小时 54 分钟 / 15 Lectures · 3 Hours 54 Minutes

- Experience and Leveling Up

  14:03

- 

  Level Up Info Data Asset

  16:10

- 

  Adding XP to the Player State

  11:05

- 

  Listening for XP Changes

  17:55

- 

  Awarding XP Game Plan

  06:51

- 

  XP Reward for Enemies

  16:36

- 

  Incoming XP Meta Attribute

  04:13

- 

  Passively Listening for Events

  18:38

- 

  Sending XP Events

  14:40

- 

  Showing XP in the HUD

  14:51

- 

  Level Up Interface Function

  16:59

- 

  Leveling Up

  21:56

- 

  Showing Level in the HUD

  24:37

- 

  Level Up Niagara System

  17:02

- 

  Level Up HUD Message

  18:17

## 22.Attribute Points / 属性点

- 6 个讲座·1小时 15 分钟 / 6 Lectures · 1 Hour 15 Minutes

- Attribute Points Member Variable

  09:11

- 

  Showing Attribute Points in the HUD

  08:25

- 

  Attribute Upgrade Buttons

  08:34

- 

  Upgrading Attributes

  18:43

- 

  Top Off Our Fluids

  14:12

- 

  Attribute Menu Polish

  16:08

## 23.Spell Menu / 法术菜单

- 23 个讲座·5 小时 45 分钟 / 23 Lectures · 5 Hours 45 Minutes

- Spell Menu Design

  06:00

- 

  Spell Globe Button

  13:10

- 

  Offensive Spell Tree

  11:45

- 

  Passive Spell Tree

  09:19

- 

  Equipped Spell Row

  16:57

- 

  Spell Menu Widget

  19:52

- 

  Spell Description Box

  12:05

- 

  Spell Menu Button

  24:29

- 

  Spell Menu Widget Controller

  28:45

- 

  Constructing the Spell Menu Widget Controller

  21:46

- 

  Equipped Row Button

  26:05

- 

  Ability Status and Type

  13:33

- 

  Showing Abilities in the Spell Tree

  24:07

- 

  Ability Level Requirement

  12:09

- 

  Update Ability Statuses

  12:34

- 

  Updating Status in the Spell Menu

  14:07

- 

  Show Spell Points

  10:10

- 

  Selecting Icons

  12:12

- 

  Deselecting Icons

  07:01

- 

  Spell Menu Buttons

  39:22

- 

  Selected Ability

  12:14

- 

  Spending Spell Points

  29:55

- 

  Rich Text Blocks

  12:59

- 

  Spell Descriptions

  19:26

- 

  FireBolt Description

  12:37

- 

  Cost and Cooldown in Spell Description

  33:40

- 

  Self Deselect

  11:17

- 

  Equipped Spell Row Animations

  17:33

- 

  Ability Types

  22:47

- 

  Equipping Abilities

  42:03

- 

  Updating the Overlay When Equipping Abilities

  09:52

- 

  Globe Reassigned

  07:11

- 

  Unbinding Delegates

  05:38

## 24.Combat Tricks / 战斗技巧

- 15 个讲座·3 小时 42 分钟 / 15 Lectures · 3 Hours 42 Minutes

- Debuff Tags

  17:45

- 

  Debuff Parameters

  06:16

- 

  Damage Effect Params Struct

  24:36

- 

  Using Damage Effect Params

  14:27

- 

  Determining Debuff

  23:40

- 

  Debuff Info in the Effect Context

  18:51

- 

  Debuff in the Attribute Set

  15:16

- 

  Dynamic Gameplay Effects

  24:36

- 

  Debuff Niagara Component

  26:40

- 

  Death Impulse Magnitude

  05:25

- 

  Death Impulse in the Effect Context

  12:30

- 

  Handling Death Impulse

  18:56

- 

  Knockback

  35:08

## 25.What a Shock / 震惊法术（示例）

- 17 个讲座·4 小时 53 分钟 / 17 Lectures · 4 Hours 53 Minutes

- FireBolt Projectile Spread

  28:39

- 

  Spawning Multiple Projectiles

  15:28

- 

  Homing Projectiles

  21:10

- 

  Click Niagara System

  03:41

- 

  Invoke Replicated Event

  24:30

- 

  Aura Beam Spell

  16:37

- 

  Electrocute Montage

  21:24

- 

  Player Block Tags

  13:58

- 

  GameplayCue Notify Paths

  13:38

- 

  Gameplay Cue Notify Actor

  19:13

- 

  Electrocute Looping Sound

  06:18

- 

  Target Trace Channel

  09:37

- 

  First Trace Target

  28:39

- 

  Additional Targets

  23:37

- 

  Shock Loop Cues on Additional Targets

  16:16

- 

  Electrocute Cost Cooldown and Damage

  08:54

- 

  Applying Electrocute Cost and Damage

  14:20

- 

  Electrocute Polish

  24:50

- 

  Explode Dem FireBoltz

  07:36

- 

  Stun

  47:55

- 

  Stun Niagara System

  16:49

- 

  Shock Loop Animations

  20:19

## 26.Passive Spells / 被动法术

- 2 个讲座·8 分钟 / 2 Lectures · 8 Minutes

- Passive Spell tags

  02:31

- 

  Aura Passive Ability

  08:08

- 

  Passive Ability Info

  07:55

- 

  Passive Tags in Spell Tree

  07:23

- 

  Multiple Level Up Rewards

  10:18

- 

  Passive Ability Activation

  37:30

- 

  Passive Niagara Component

  30:31

## 27.Arcane Shards / 奥术碎片

- 18 个讲座·4 小时 39 分钟 / 18 Lectures · 4 Hours 39 Minutes

- Magic Circle

  12:25

- 

  Spawning Magic Circles

  11:35

- 

  Magic Circle Interface Functions

  09:56

- 

  Arcane Shards Spell

  13:25

- 

  Wait Input Press

  08:58

- 

  Anti Aliasing and Moving Decals

  04:10

- 

  Point Collection

  46:45

- 

  Async Point Locations

  10:21

- 

  Gameplay Cue Notify Burst

  17:26

- 

  Arcane Shards Montage

  14:36

- 

  Radial Damage Parameters

  19:29

- 

  Setting Radial Damage Parameters

  10:18

- 

  Radial Damage with Falloff

  21:18

- 

  Tying Radial Damage All Together

  27:10

- 

  Ignore Enemies while Magic Circle Active

  06:52

- 

  Knockback Force and Death Impulse Overrides

  15:50

- 

  Spell Descriptions

  13:45

- 

  Arcane Shards Cost and Cooldown

  14:23

## 28.Fire Blast / 火焰冲击

- 9 个讲座·1小时 59 分钟 / 9 Lectures · 1 Hour 59 Minutes

- FireBlast Ability

  12:36

- 

  FireBlast Cost and Cooldown

  05:44

- 

  Aura Fire Ball

  12:21

- 

  Spawning FireBalls

  10:02

- 

  FireBall Timelines

  17:39

- 

  Causing FireBall Damage

  10:51

- 

  FireBall Explosive Damage

  31:26

- 

  Empty Cooldown Texture

  05:26

- 

  Execute Local Gameplay Cues

  12:43

## 29.Saving Progress / 保存进度

- 23 个讲座·5 小时 45 分钟 / 23 Lectures · 5 Hours 45 Minutes

- Saving Progress

  08:54

- 

  Main Menu

  33:09

- 

  Play and Quit Buttons

  12:20

- 

  Vacant Load Slot

  13:41

- 

  Enter Name Load Slot

  03:42

- 

  Taken Load Slot

  05:07

- 

  Load Menu

  11:40

- 

  MVVM

  07:45

- 

  Changes Needed for 5.3+

  34:30

- 

  View Model Class

  13:21

- 

  Constructing a View Model

  19:31

- 

  Load Slot View Model

  24:19

- 

  Switching the Widget Switcher

  19:38

- 

  Save Game Object

  15:10

- 

  Binding Variables to ViewModels

  11:15

- 

  Load Slot Status

  17:19

- 

  Enabling the Select Slot Button

  09:12

- 

  Enabling Play and Delete Buttons

  08:34

- 

  Are You Sure Widget

  24:17

- 

  Deleting a Slot

  16:48

- 

  Map Name Field Notify

  16:21

- 

  Saving the Map Name

  04:17

- 

  Traveling to the Saved Map

  13:42

## 30.Checkpoints / 检查点

- 15 个讲座·3 小时 42 分钟 / 15 Lectures · 3 Hours 42 Minutes

- Choosing the Player Start

  09:34

- 

  Setting the Default Player Start

  12:50

- 

  Save the Player Start Tag

  07:49

- 

  Checkpoints

  21:18

- 

  Interface Function for Saving Progress

  19:45

- 

  Saving Player Data

  08:33

- 

  Loading Player Data

  15:34

- 

  Initializing Attributes From Disk

  24:30

- 

  Showing Player Level in Load Screen

  06:19

- 

  Saving Abilities

  14:46

- 

  Notes on this lecture

  00:05

- 

  Loading Abilities

  29:41

- 

  Data Structures for Saving Data

  13:15

- 

  Saving World State

  21:47

- 

  Loading World State

  15:46

## 31.Map Entrance / 地图入口

- 17 个讲座·4 小时 53 分钟 / 17 Lectures · 4 Hours 53 Minutes

- Different Highlight Colors

  10:10

- 

  Highlight Interface

  06:47

- 

  Targeting Status

  23:49

- 

  Highlighting Non-Enemies

  06:15

- 

  Set Move-To Location

  15:50

- 

  Beacons

  15:49

- 

  Map Entrance

  31:09

- 

  Dungeon Stair Entrance

  14:04

- 

  Dungeon Entrance Blueprints

  14:50

- 

  Polish Menu

  04:58

- 

  Spawn Volumes

  24:54

- 

  Player Death

  18:10

- 

  Loot Tiers

  38:41

- 

  Loot Effects

  21:06

- 

  Loot Drop Curve

  12:41

- 

  Pickup Sounds

  12:46

- 

  Quit Button

  20:51

## 32.Course Conclusion / 课程总结

- 2 个讲座·8 分钟 / 2 Lectures · 8 Minutes

- Quest - Levels

  01:20

- 

  Conclusion - Bonus Video

  06:24
